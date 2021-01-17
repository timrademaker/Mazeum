// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameMode.generated.h"

UCLASS(MinimalAPI)
class AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AStealthGameMode();

    virtual void StartPlay() override;
    virtual void ResetLevel() override;

    /** 
     * Register an array of guard patrol path components that are part of a room or section of the level
     * @param PatrolPaths An array of patrol path components, of which one is used when spawning a guard
     */
    void RegisterGuardPatrolPathArray(TArray<class UGuardPatrolPathComponent*>* const PatrolPaths);

    /**
     * Pause or unpause the game
     * @param NewPauseState If true, the game will be paused.
     */
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Pause Unpause", ToolTip = "Pause or unpause the game"))
    void SetPauseState(bool NewPauseState);

    /** Toggle the pause state of the game */
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Pause Unpause", ToolTip = "Toggle the pause state of the game"))
    void TogglePause();

protected:
    /** Callback for alarm components when the player is spotted */
    void OnAlarmTriggered();

    /** Callback for the target item being picked up */
    void OnTargetItemPickedUp();

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
    TArray<TArray<class UGuardPatrolPathComponent*>*> SelectPatrolPathArrays(const uint8 MaxGuardPathCount, const int32 RandomSeed);

    /**
     * Spawn guard on the patrol paths
     * @param RandomSeed The seed to use for randomisation. Leave at 0 to use a random seed.
     */
    void SpawnGuards(const TArray<TArray<class UGuardPatrolPathComponent*>*> PathArrays, const int32 RandomSeed);

    /**
     * Create a FRandomStream with the specified seed. If the seed is 0, a random seed is used.
     * @param RandomSeed The seed to use for the stream
     */
    FRandomStream CreateRandomStream(const int32 RandomSeed);

    /** Called when the player wins */
    void OnGameWin();

    /** Called when the player loses */
    void OnGameLose();

public:
    /** The maximum number of guards to spawn */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The maximum number of guards to spawn"))
    uint8 MaximumGuardCount = 10;

    /** The guard class to use when spawning guards */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The guard class to use when spawning guards"))
    TSubclassOf<class AGuard> GuardClass;

    /** The widget to display when the player wins */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The widget to display when the player wins"))
    TSubclassOf<UUserWidget> WinWidget;

    /** The widget to display when the player loses */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The widget to display when the player loses"))
    TSubclassOf<UUserWidget> LoseWidget;

    /** The widget to display when the player pauses the game */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The widget to display when the player pauses the game"))
    TSubclassOf<UUserWidget> PauseWidget;

private:
    /** The seed to use for random streams */
    int32 RandomisationSeed = 0;
    /** An array of all possible patrol path arrays. One array represents a single room. */
    TArray<TArray<class UGuardPatrolPathComponent*>*> PatrolPathArrays;

    /** The player's PlayerController */
    APlayerController* PlayerController = nullptr;

    /** The widget that is displayed when the game ended and the player won */
    UUserWidget* WinWidgetInstance = nullptr;

    /** The widget that is displayed when the game ended and the player lost */
    UUserWidget* LoseWidgetInstance = nullptr;

    /** The GameMode's instance of the pause widget */
    UUserWidget* PauseWidgetInstance = nullptr;
};
