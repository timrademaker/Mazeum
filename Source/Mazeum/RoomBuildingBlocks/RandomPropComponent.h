// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PropSizeEnum.h"

#include "CoreMinimal.h"
#include "BuildingBlockMeshComponent.h"
#include "RandomPropComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAZEUM_API URandomPropComponent : public UBuildingBlockMeshComponent
{
	GENERATED_BODY()
	
public:
    /** The size of the prop to place at this location */
    UPROPERTY(EditAnywhere, meta = (ToolTip = "The size of the prop to place at this location"))
    EPropSize PropSize;
};
