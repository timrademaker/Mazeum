// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameMode.generated.h"

class UGuardPatrolPathComponent;
class AGuard;

UCLASS(MinimalAPI)
class AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AStealthGameMode();

    virtual void StartPlay() override;

    /** 
     * Register an array of guard patrol path components that are part of a room or section of the level
     * @param PatrolPaths An array of patrol path components, of which one is used when spawning a guard
     */
    void RegisterGuardPatrolPathArray(TArray<UGuardPatrolPathComponent*>* const PatrolPaths);
	

protected:
    /** Callback for alarm components when the player is spotted */
    void OnAlarmTriggered();

private:
    /**
     * Set a random seed to be used for guard spawning
     * @param RandomSeed The seed to use for random functions in this class from now on. Leave at 0 to use a random seed.
     */
    void SetRandomSeed(const int32 RandomSeed);

    /**
     * Determine which patrol path arrays to use when spawning guards
     * @param MaxGuardCount The maximum number of guard paths to choose
     * @param RandomSeed The seed to use for randomisation. Leave at 0 to use a random seed.
     */
    TArray<TArray<UGuardPatrolPathComponent*>*> SelectPatrolPathArrays(const uint8 MaxGuardPathCount, const int32 RandomSeed);

    /**
     * Spawn guard on the patrol paths
     * @param RandomSeed The seed to use for randomisation. Leave at 0 to use a random seed.
     */
    void SpawnGuards(const TArray<TArray<UGuardPatrolPathComponent*>*> PathArrays, const int32 RandomSeed);

    /**
     * Create a FRandomStream with the specified seed. If the seed is 0, a random seed is used.
     * @param RandomSeed The seed to use for the stream
     */
    FRandomStream CreateRandomStream(const int32 RandomSeed);

public:
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The maximum number of guards to spawn"))
    uint8 MaximumGuardCount = 10;

    UPROPERTY(EditAnywhere, meta = (ToolTip = "The guard class to use when spawning guards"))
    TSubclassOf<AGuard> GuardClass;

private:
    /** The seed to use for random streams */
    int32 RandomisationSeed = 0;
    /** An array of all possible patrol path arrays. One array represents a single room. */
    TArray<TArray<UGuardPatrolPathComponent*>*> PatrolPathArrays;
};
