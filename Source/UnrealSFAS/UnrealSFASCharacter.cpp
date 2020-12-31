// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSFASCharacter.h"

#include "InteractableInterface.h"
#include "StealthGameMode.h"
#include "Gameplay/PickUpBase.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealSFASCharacter

AUnrealSFASCharacter::AUnrealSFASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AUnrealSFASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Find all actors implementing an interface indicating that the player can interact with the actor
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UInteractableInterface::StaticClass(), Interactables);

	StartTransform = GetActorTransform();
}

void AUnrealSFASCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Placeholder until I add animations
	GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 0.5f));
}

void AUnrealSFASCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Placeholder until I add animations
	GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealSFASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealSFASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealSFASCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealSFASCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealSFASCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealSFASCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealSFASCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealSFASCharacter::OnResetVR);

	// Interaction with items
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnrealSFASCharacter::OnInteract);
	PlayerInputComponent->BindAction("DropHeldItem", IE_Pressed, this, &AUnrealSFASCharacter::OnDropHeldItem);

	// Crouching
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUnrealSFASCharacter::OnCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUnrealSFASCharacter::OnUnCrouch);

	// Pausing
	FInputActionBinding& pauseToggle = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AUnrealSFASCharacter::OnPause);
	pauseToggle.bExecuteWhenPaused = true;
}

float AUnrealSFASCharacter::CalculateMovementSpeedModifier() const
{
	if (!HeldItem)
	{
		return 1.0f;
	}
	else
	{
		const float weight = HeldItem->GetItemWeight();
		const float modifier = 1.0f - weight / MaximumItemWeight;
		return FMath::Clamp(modifier, MinimumMovementSpeedModifier, 1.0f);
	}
}

void AUnrealSFASCharacter::Reset()
{
	SetActorTransform(StartTransform);
}

void AUnrealSFASCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealSFASCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealSFASCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealSFASCharacter::OnInteract()
{
	IInteractableInterface* nearestInteractableInRange = nullptr;
	float distanceToNearestInteractable = FLT_MAX;

	// Find the nearest interactable
	for (auto& interactable : Interactables)
	{
		float distance = FVector::Dist(GetActorLocation(), interactable->GetActorLocation());
		if (distance <= InteractionRange)
		{
			if (distance < distanceToNearestInteractable)
			{
				distanceToNearestInteractable = distance;
				nearestInteractableInRange = Cast<IInteractableInterface>(interactable);
			}
		}
	}

	// Interact with the nearest interactable
	if (nearestInteractableInRange)
	{
		nearestInteractableInRange->Interact(this);
		
		if (!HeldItem)
		{
			// If the interactable is derived from APickUpBase, we now hold the item.
			HeldItem = Cast<APickUpBase>(nearestInteractableInRange);
		}
	}
}

void AUnrealSFASCharacter::OnDropHeldItem()
{
	if (HeldItem)
	{
		HeldItem->DropItem();
		HeldItem = nullptr;
	}
}

void AUnrealSFASCharacter::OnCrouch()
{
	Crouch();
}

void AUnrealSFASCharacter::OnUnCrouch()
{
	UnCrouch();
}

void AUnrealSFASCharacter::OnPause()
{
	AStealthGameMode* gm = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		gm->TogglePause();
	}
}

void AUnrealSFASCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealSFASCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealSFASCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value * CalculateMovementSpeedModifier());
	}
}

void AUnrealSFASCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value * CalculateMovementSpeedModifier());
	}
}
