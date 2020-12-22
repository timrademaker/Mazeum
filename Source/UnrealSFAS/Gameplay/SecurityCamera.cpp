// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCamera.h"

#include "AlarmComponent.h"

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

	AlarmComponent = CreateDefaultSubobject<UAlarmComponent>("Alarm");

	CameraAreaDecal->DecalSize = FVector(CameraVisionRadius);

	VisionCollisionShape.SetCapsule(CameraVisionRadius, 50.0f);
}

// Called when the game starts or when spawned
void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	if (SplineEndWaitTime < 0.0f)
	{
		PathSpline->SetClosedLoop(true);
	}

	CameraMovementSpeed = PathSpline->Duration / SplinePathDuration;

	PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ASecurityCamera::UpdateCameraTargetPosition(float DeltaTime)
{
	if (ReverseCountDown <= 0.0f)
	{
		CurrentSplineTime += CameraMovementSpeed * DeltaTime * (IsMovingBackToSplineStart ? -1.0f : 1.0f);
		const FVector targetPosition = PathSpline->GetLocationAtTime(CurrentSplineTime, ESplineCoordinateSpace::World);
		CameraAreaDecal->SetWorldLocation(targetPosition);

		// Check if we reached the end of the spline in either direction
		if (CurrentSplineTime >= PathSpline->Duration || CurrentSplineTime <= 0.0f)
		{
			if (PathSpline->IsClosedLoop())
			{
				// Set this back to 0, as we're not reversing along the spline.
				CurrentSplineTime = 0.0f;
			}
			else
			{
				CurrentSplineTime = FMath::Clamp(CurrentSplineTime, 0.0f, PathSpline->Duration);
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

bool ASecurityCamera::CameraHasSpottedPlayer()
{
	const FVector traceStart = GetActorLocation();
	const FVector traceEnd = PathSpline->GetLocationAtTime(CurrentSplineTime, ESplineCoordinateSpace::World);
	const FQuat traceRotaton = UKismetMathLibrary::FindLookAtRotation(traceStart, traceEnd).Quaternion();
	FHitResult hitResult;

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepSingleByChannel(hitResult, traceStart, traceEnd, traceRotaton, ECollisionChannel::ECC_Camera, VisionCollisionShape, queryParams))
	{
		if (hitResult.GetActor() == PlayerPawn)
		{
			return true;
		}
	}

	return false;
}

// Called every frame
void ASecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraTargetPosition(DeltaTime);

	if (CameraHasSpottedPlayer())
	{
		AlarmComponent->TriggerAlarm();
	}
}

void ASecurityCamera::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CameraAreaDecal->DecalSize = FVector(CameraVisionRadius);
}

