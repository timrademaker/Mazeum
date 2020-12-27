// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "AlarmComponent.h"
#include "GuardPatrolPath.h"

#include "Components/SplineComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGuard::AGuard()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AlarmComponent = CreateDefaultSubobject<UAlarmComponent>("Alarm");

	VisionCollisionShape.SetCapsule(VisionRadius, 50.0f);
	CollisionQueryParams.AddIgnoredActor(this);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGuard::SetPatrolPath(UGuardPatrolPathComponent* NewPatrolPath, bool AbortCurrentPath)
{
	NextPatrolPath = NewPatrolPath;
	
	if (AbortCurrentPath)
	{
		StartNextPatrolPath();
	}
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void AGuard::StartNextPatrolPath()
{
	CurrentTimeAlongPatrolPath = 0.0f;
	if (NextPatrolPath)
	{
		CurrentPatrolPath = NextPatrolPath;
		NextPatrolPath = nullptr;

		if (CurrentPatrolPath->PathEndWaitTime < 0.0f)
		{
			CurrentPatrolPath->SetClosedLoop(true);
		}

		PatrolSpeed = CurrentPatrolPath->GetSplineLength() / CurrentPatrolPath->PathDuration;
		SplineAdvancementRate = CurrentPatrolPath->Duration / CurrentPatrolPath->PathDuration;
	}
}

void AGuard::Patrol(float DeltaTime)
{
	if (!CurrentPatrolPath)
	{
		StartNextPatrolPath();
	}

	if (!CurrentPatrolPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("No patrol path"));
		return;
	}

	if (ReverseCountDown <= 0.0f)
	{
		CurrentTimeAlongPatrolPath += SplineAdvancementRate * DeltaTime * (IsPatrollingBackwards ? -1.0f : 1.0f);
		CurrentTimeAlongPatrolPath = FMath::Clamp(CurrentTimeAlongPatrolPath, 0.0f, CurrentPatrolPath->Duration);

		const FVector targetPosition = CurrentPatrolPath->GetLocationAtTime(CurrentTimeAlongPatrolPath, ESplineCoordinateSpace::World);
		
		// Move the guard
		FVector movementDirection = targetPosition - GetActorLocation();
		movementDirection.Normalize();
		AddMovementInput(PatrolSpeed * DeltaTime * movementDirection);

		// Check if we reached the end of the spline in either direction
		if (CurrentTimeAlongPatrolPath >= CurrentPatrolPath->Duration || CurrentTimeAlongPatrolPath <= 0.0f)
		{
			if (CurrentPatrolPath->IsClosedLoop())
			{
				// Set this back to 0, as we're not reversing along the spline.
				CurrentTimeAlongPatrolPath = 0.0f;
			}
			else
			{
				ReverseCountDown = CurrentPatrolPath->PathEndWaitTime;
				IsPatrollingBackwards = !IsPatrollingBackwards;
			}

			if (CurrentTimeAlongPatrolPath == 0.0f)
			{
				StartNextPatrolPath();
			}
		}
	}
	else
	{
		ReverseCountDown -= DeltaTime;
	}
}

bool AGuard::GuardCanSeePlayer()
{
	if ((GetActorLocation() - PlayerPawn->GetActorLocation()).SizeSquared() > VisionRange* VisionRange)
	{
		return false;
	}

	const FVector traceStart = GetActorLocation();
	const FVector traceEnd = traceStart + GetActorForwardVector() * VisionRange;
	const FQuat traceRotaton = UKismetMathLibrary::FindLookAtRotation(traceStart, traceEnd).Quaternion();
	FHitResult hitResult;

	if (GetWorld()->SweepSingleByChannel(hitResult, traceStart, traceEnd, traceRotaton, ECollisionChannel::ECC_Camera, VisionCollisionShape, CollisionQueryParams))
	{
		if (hitResult.GetActor() == PlayerPawn)
		{
			return true;
		}
	}

	return false;
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Patrol(DeltaTime);
	
	if (GuardCanSeePlayer())
	{
		AlarmComponent->TriggerAlarm();
	}
}

