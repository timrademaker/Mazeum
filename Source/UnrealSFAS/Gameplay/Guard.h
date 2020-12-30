// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GuardPatrolPath.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

class UGuardPatrolPathComponent;
class UAlarmComponent;

UCLASS()
class UNREALSFAS_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 
	 * Set a new patrol path for a guard 
	 * @param NewPatrolPath The new patrol path
	 * @param AbortCurrentPath If true, the guard will immediately start following the new path. This might lead to unexpected behaviour, like bumping into objects.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPatrolPath(UGuardPatrolPathComponent* NewPatrolPath, bool AbortCurrentPath = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Reset() override;

private:
	/** Start the guard's next patrol path, if there is one. */
	void StartNextPatrolPath();

	/** Move the guard along its path */
	void Patrol(float DeltaTime);

	/** Check if the guard can see an actor
	 * @param Actor The actor for which to check if the guard is able to see it
	 * @returns True if the guard is able to see the actor
	 */
	bool GuardCanSeeActor(const AActor* Actor);

public:
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The guard's field of view in degrees", ClampMin = "0.0", ClampMax = "180.0"))
	float FieldOfView = 75.0f;
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The range at which the guard can spot the player", ClampMin = "0.0"))
	float VisionRange = 500.0f;

private:
	/** The path the guard is currently patrolling */
	UGuardPatrolPathComponent* CurrentPatrolPath = nullptr;
	/** The path the guard will patrol after the current path is done */
	UGuardPatrolPathComponent* NextPatrolPath = nullptr;

	/** The speed at which the guard moves */
	float PatrolSpeed = 0.0f;
	/** The length of the current patrol path */
	float PathLength = 0.0f;

	/** How far the guard is along the current path */
	float CurrentDistanceAlongPatrolPath = 0.0f;
	/** The time until the guard starts moving again afer reaching the end of the path */
	float ReverseCountDown = 0.0f;
	/** True if the guard is on its way back to the start of its path */
	bool IsPatrollingBackwards = false;

	/** The cosine of half of the guard's field of view, used to determine if the guard can see an actor */
	float HalfFieldOfViewCosine = 0.0f;
	/** The parameters used to check if an actor can be seen */
	FCollisionQueryParams CollisionQueryParams;

	/** The player's pawn */
	APawn* PlayerPawn = nullptr;
	/** The guard's alarm component */
	UAlarmComponent* AlarmComponent = nullptr;
};
