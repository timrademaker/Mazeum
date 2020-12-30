// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCamera.h"

#include "AlarmComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASecurityCamera::ASecurityCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	PathSpline = CreateDefaultSubobject<USplineComponent>("PathSpline");
	PathSpline->SetupAttachment(RootComponent);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>("CameraMesh");
	CameraMesh->SetupAttachment(RootComponent);

	CameraAreaDecal = CreateDefaultSubobject<UDecalComponent>("CameraViewDecal");
	CameraAreaDecal->SetupAttachment(RootComponent);
	CameraAreaDecal->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f));

	UArrowComponent* arrowComponent = CreateDefaultSubobject<UArrowComponent>("CameraForward");
	arrowComponent->SetupAttachment(RootComponent);

	AlarmComponent = CreateDefaultSubobject<UAlarmComponent>("Alarm");

	CollisionQueryParams.AddIgnoredActor(this);

	HalfFieldOfViewCosine  = FMath::Cos(FMath::DegreesToRadians(FieldOfView / 2.0f));
	HalfFieldOfViewTangent = FMath::Tan(FMath::DegreesToRadians(FieldOfView / 2.0f));
}

// Called when the game starts or when spawned
void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	if (SplineEndWaitTime < 0.0f)
	{
		PathSpline->SetClosedLoop(true);
	}

	SplineLength = PathSpline->GetSplineLength();
	CameraMovementSpeed = SplineLength / SplinePathDuration;

	PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ASecurityCamera::UpdateCameraTargetPosition(float DeltaTime)
{
	if (ReverseCountDown <= 0.0f)
	{
		CurrentDistanceAlongSpline += CameraMovementSpeed * DeltaTime * (IsMovingBackToSplineStart ? -1.0f : 1.0f);
		CurrentDistanceAlongSpline = FMath::Clamp(CurrentDistanceAlongSpline, 0.0f, SplineLength);

		const FVector targetPosition = PathSpline->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		CameraAreaDecal->SetWorldLocation(targetPosition);

		CurrentCameraRange = (targetPosition - GetActorLocation()).Size() * HalfFieldOfViewCosine;

		// Update decal size
		const float distanceToTargetPosition = CurrentCameraRange;
		const float decalDiameter = distanceToTargetPosition * HalfFieldOfViewTangent * 2.0f;
		CameraAreaDecal->DecalSize = FVector(decalDiameter);
		// Rotate decal based on camera direction
		CameraAreaDecal->SetWorldRotation(FRotator(90.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetPosition).Yaw, 0.0f));

		// Check if we reached the end of the spline in either direction
		if (CurrentDistanceAlongSpline >= SplineLength || CurrentDistanceAlongSpline <= 0.0f)
		{
			if (PathSpline->IsClosedLoop())
			{
				// Set this back to 0, as we're not reversing along the spline.
				CurrentDistanceAlongSpline = 0.0f;
			}
			else
			{
				ReverseCountDown = SplineEndWaitTime;
				IsMovingBackToSplineStart = !IsMovingBackToSplineStart;
			}
		}
	}
	else
	{
		ReverseCountDown -= DeltaTime;
	}
}

bool ASecurityCamera::CameraCanSeeActor(const AActor* TargetActor)
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Passed a nullptr to CameraCanSeeActor"));
		return false;
	}

	const FVector ownLocation = GetActorLocation();
	const FVector targetLocation = TargetActor->GetActorLocation();

	if ((targetLocation - ownLocation).SizeSquared() > CurrentCameraRange * CurrentCameraRange)
	{
		return false;
	}

	const FVector cameraForward = UKismetMathLibrary::GetDirectionUnitVector(ownLocation, PathSpline->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World));
	const FVector directionToActor = UKismetMathLibrary::GetDirectionUnitVector(ownLocation, targetLocation);

	const float directionForwardDot = FVector::DotProduct(cameraForward, directionToActor);
	if (directionForwardDot >= HalfFieldOfViewCosine)
	{
		// Check if the actor is behind a wall
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, ownLocation, targetLocation, ECollisionChannel::ECC_Camera, CollisionQueryParams))
		{
			if (hitResult.Actor != PlayerPawn)
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

// Called every frame
void ASecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraTargetPosition(DeltaTime);

	if (CameraCanSeeActor(PlayerPawn))
	{
		AlarmComponent->TriggerAlarm();
	}
}

void ASecurityCamera::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateCameraTargetPosition(0.0f);
}

void ASecurityCamera::Reset()
{
	Super::Reset();

	CurrentDistanceAlongSpline = 0.0f;
	UpdateCameraTargetPosition(0.0f);
}
