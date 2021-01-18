// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBlockActorBase.h"

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingBlockMeshComponent.generated.h"

/** An enum representing different building block types */
UENUM()
enum class EBuildingBlockType : uint8
{
    Other = 0,
    Door,
    Vent,
    Stealable
};

/** A component that can be used to create a room */
UCLASS()
class MAZEUM_API UBuildingBlockMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
    /** The actor equivalent of this component */
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The actor equivalent of this component. In the case of a door mesh, this would be the actual door."))
    TSubclassOf<ABuildingBlockActorBase> ActorEquivalent;

    /** The type of building block this component represents */
    UPROPERTY(EditDefaultsOnly)
    EBuildingBlockType BuildingBlockType;
};
