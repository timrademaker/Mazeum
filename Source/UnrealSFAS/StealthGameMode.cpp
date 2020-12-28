// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameMode.h"

#include "Gameplay/AlarmComponent.h"
#include "Gameplay/Guard.h"
#include "Gameplay/GuardPatrolPath.h"
#include "Gameplay/PickUpBase.h"

#include "Blueprint/UserWidget.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"


AStealthGameMode::AStealthGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AStealthGameMode::StartPlay()
{
    Super::StartPlay();

    if (!GuardClass)
    {
        GuardClass = AGuard::StaticClass();
    }

    {
        TArray<AActor*> pickups;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickUpBase::StaticClass(), pickups);

        // Subscribe to all pick-up events
        for (AActor* pickup : pickups)
        {
            Cast<APickUpBase>(pickup)->OnItemPickedUp().AddUObject(this, &AStealthGameMode::OnTargetItemPickedUp);
        }
    }

    if(PatrolPathArrays.Num() > 0)
    {
        TArray<TArray<UGuardPatrolPathComponent*>*> pathArrays = SelectPatrolPathArrays(MaximumGuardCount, RandomisationSeed);
        SpawnGuards(pathArrays, RandomisationSeed);
    }

    {
        TArray<AActor*> worldActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), worldActors);

        // Subscribe to all alarm events
        for (const AActor* actor : worldActors)
        {
            UActorComponent* alarmComponent = actor->GetComponentByClass(UAlarmComponent::StaticClass());

            if (alarmComponent)
            {
                Cast<UAlarmComponent>(alarmComponent)->OnAlarmTriggered().AddUObject(this, &AStealthGameMode::OnAlarmTriggered);
            }
        }
    }

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AStealthGameMode::RegisterGuardPatrolPathArray(TArray<UGuardPatrolPathComponent*>* const PatrolPaths)
{
    if (PatrolPaths->Num() > 0)
    {
        PatrolPathArrays.Add(PatrolPaths);
    }
}

void AStealthGameMode::OnAlarmTriggered()
{
    OnGameLose();
}

void AStealthGameMode::OnTargetItemPickedUp()
{
    OnGameWin();
}

void AStealthGameMode::SetRandomSeed(const int32 RandomSeed)
{
    RandomisationSeed = RandomSeed;
}

TArray<TArray<UGuardPatrolPathComponent*>*> AStealthGameMode::SelectPatrolPathArrays(const uint8 MaxGuardPathCount, const int32 RandomSeed)
{
    const uint8 numGuardsToSpawn = FMath::Min(PatrolPathArrays.Num(), static_cast<int32>(MaxGuardPathCount));

    // All possible arrays to spawn a guard on
    TArray<TArray<UGuardPatrolPathComponent*>*> possibleArrays = PatrolPathArrays;

    // Early out if all paths will be used
    if (numGuardsToSpawn == possibleArrays.Num())
    {
        return possibleArrays;
    }

    FRandomStream randomStream = CreateRandomStream(RandomSeed);

    // The arrays to spawn guards for
    TArray<TArray<UGuardPatrolPathComponent*>*> patrolArraysToUse;

    for (uint8 i = 0; i < numGuardsToSpawn; ++i)
    {
        const int32 arrayIndex = randomStream.RandRange(0, possibleArrays.Num() - 1);
        patrolArraysToUse.Add(possibleArrays[arrayIndex]);
        possibleArrays.RemoveAt(arrayIndex);
    }

    return patrolArraysToUse;
}

void AStealthGameMode::SpawnGuards(const TArray<TArray<UGuardPatrolPathComponent*>*> PathArrays, const int32 RandomSeed)
{
    FRandomStream randomStream = CreateRandomStream(RandomSeed);

    AGuard* guard = nullptr;
    FVector spawnLocation;
    FVector spawnRotation;

    for (TArray<UGuardPatrolPathComponent*>* pathArray : PathArrays)
    {
        // Select random path
        const int32 pathIndex = randomStream.RandRange(0, pathArray->Num() - 1);
        UGuardPatrolPathComponent* pathToUse = (*pathArray)[pathIndex];
        
        // Spawn guard at path start
        spawnLocation = pathToUse->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
        spawnRotation = pathToUse->GetDirectionAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
        
        guard = GetWorld()->SpawnActor<AGuard>(GuardClass, spawnLocation, spawnRotation.Rotation());
        guard->SetPatrolPath(pathToUse, true);
    }
}

FRandomStream AStealthGameMode::CreateRandomStream(const int32 RandomSeed)
{
    return FRandomStream(RandomSeed == 0 ? FMath::Rand() : RandomSeed);
}

void AStealthGameMode::OnGameWin()
{
    // Show win widget and pause game
    UUserWidget* winWidget = CreateWidget(GetWorld(), WinWidget);
    winWidget->AddToViewport(0);

    PlayerController->SetPause(true);
    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->bShowMouseCursor = true;
}

void AStealthGameMode::OnGameLose()
{
    // Show lose widget and pause game
    UUserWidget* loseWidget = CreateWidget(GetWorld(), LoseWidget);
    loseWidget->AddToViewport(0);

    PlayerController->SetPause(true);
    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->bShowMouseCursor = true;
}
