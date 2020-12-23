// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GuardPatrolPath.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

struct FGuardPatrolPath;
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

	/** TODO: Remove this function, as it's only here for testing **/
	UFUNCTION(BlueprintCallable)
	void SetPatrolPath(const FGuardPatrolPath& NewPatrolPath, bool AbortCurrentPath = false);

	/** 
	 * Set a new patrol path for a guard 
	 * @param NewPatrolPath The new patrol path
	 * @param AbortCurrentPath If true, the guard will immediately start following the new path. This might lead to unexpected behaviour, like bumping into objects.
	 */
	void SetPatrolPath(FGuardPatrolPath* NewPatrolPath, bool AbortCurrentPath = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** Start the guard's next patrol path, if there is one. */
	void StartNextPatrolPath();

	/** Move the guard along its path */
	void Patrol(float DeltaTime);

	/** Check if the guard can see the player
	 * @returns True if the guard is able to see the player
	 */
	bool GuardCanSeePlayer();

public:
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The radius of the capsule which the guard uses as its vision capsule", ClampMin = "0.0"))
	float VisionRadius = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The range at which the guard can spot the player", ClampMin = "0.0"))
	float VisionRange = 500.0f;

private:
	/** The path the guard is currently patrolling */
	FGuardPatrolPath* CurrentPatrolPath;
	/** The path the guard will patrol after the current path is done */
	FGuardPatrolPath* NextPatrolPath;

	/** The speed at which the guard moves */
	float PatrolSpeed = 0.0f;
	/** How far the guard moves along the spline per second, in spline time */
	float SplineAdvancementRate = 0.0f;

	/** How far the guard is along the current path */
	float CurrentTimeAlongPatrolPath = 0.0f;
	/** The time until the guard starts moving again afer reaching the end of the path */
	float ReverseCountDown = 0.0f;
	/** True if the guard is on its way back to the start of its path */
	bool IsPatrollingBackwards = false;

	/** The shape to use as the guard's detection */
	FCollisionShape VisionCollisionShape;
	/** The parameters used for the guard's vision */
	FCollisionQueryParams CollisionQueryParams;

	/** The player's pawn */
	APawn* PlayerPawn;
	/** The guard's alarm component */
	UAlarmComponent* AlarmComponent;
};
