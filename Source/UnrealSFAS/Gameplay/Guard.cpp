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

void AGuard::Reset()
{
	Super::Reset();

	if (CurrentPatrolPath)
	{
		SetActorLocation(CurrentPatrolPath->GetLocationAtTime(0.0f, ESplineCoordinateSpace::World));
		NextPatrolPath = CurrentPatrolPath;
		StartNextPatrolPath();
	}
}

void AGuard::StartNextPatrolPath()
{
	CurrentDistanceAlongPatrolPath = 0.0f;
	if (NextPatrolPath)
	{
		CurrentPatrolPath = NextPatrolPath;
		NextPatrolPath = nullptr;

		if (CurrentPatrolPath->PathEndWaitTime < 0.0f)
		{
			CurrentPatrolPath->SetClosedLoop(true);
		}

		PathLength = CurrentPatrolPath->GetSplineLength();
		PatrolSpeed = PathLength / CurrentPatrolPath->PathDuration;
	}

	HalfFieldOfViewCosine = FMath::Cos(FMath::DegreesToRadians(FieldOfView / 2.0f));
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
		CurrentDistanceAlongPatrolPath += PatrolSpeed * DeltaTime * (IsPatrollingBackwards ? -1.0f : 1.0f);
		CurrentDistanceAlongPatrolPath = FMath::Clamp(CurrentDistanceAlongPatrolPath, 0.0f, PathLength);

		const FVector targetPosition = CurrentPatrolPath->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongPatrolPath, ESplineCoordinateSpace::World);
		
		// Move the guard
		FVector movementDirection = targetPosition - GetActorLocation();
		movementDirection.Normalize();
		AddMovementInput(PatrolSpeed * DeltaTime * movementDirection);

		// Check if we reached the end of the spline in either direction
		if (CurrentDistanceAlongPatrolPath >= PathLength || CurrentDistanceAlongPatrolPath <= 0.0f)
		{
			if (CurrentPatrolPath->IsClosedLoop())
			{
				// Set this back to 0, as we're not reversing along the spline.
				CurrentDistanceAlongPatrolPath = 0.0f;
			}
			else
			{
				ReverseCountDown = CurrentPatrolPath->PathEndWaitTime;
				IsPatrollingBackwards = !IsPatrollingBackwards;
			}

			if (CurrentDistanceAlongPatrolPath == 0.0f)
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

bool AGuard::GuardCanSeeActor(const AActor* TargetActor)
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Passed a nullptr to GuardCanSeeActor"));
		return false;
	}

	const FVector ownLocation = GetActorLocation();
	const FVector targetLocation = TargetActor->GetActorLocation();

	// Check if the actor is in the guard's range
	if ((targetLocation - ownLocation).SizeSquared() > VisionRange * VisionRange)
	{
		return false;
	}

	const FVector guardForward = GetActorForwardVector();
	const FVector directionToActor = UKismetMathLibrary::GetDirectionUnitVector(ownLocation, targetLocation);
	
	const float directionForwardDot = FVector::DotProduct(guardForward, directionToActor);

	// Check if the actor is within the guard's field of view
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
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Patrol(DeltaTime);
	
	if (GuardCanSeeActor(PlayerPawn))
	{
		AlarmComponent->TriggerAlarm();
	}
}

