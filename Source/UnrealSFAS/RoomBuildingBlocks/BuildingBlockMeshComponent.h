// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBlockActorBase.h"

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingBlockMeshComponent.generated.h"

UENUM()
enum class EBuildingBlockType : uint8
{
    Other = 0,
    Door,
    Vent
};

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBuildingBlockMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The actor equivalent of this component. In the case of a door mesh, this would be the actual door."))
    TSubclassOf<ABuildingBlockActorBase> ActorEquivalent;

    UPROPERTY(EditDefaultsOnly)
    EBuildingBlockType BuildingBlockType;
};
