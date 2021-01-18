// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RoomTemplate.h"
#include "RoomBuildingBlocks/BuildingBlockMeshComponent.h"

#include "Engine/DataTable.h"

#include "BuildingBlockPlacementStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBuildingBlockPlacementStruct : public FTableRowBase
{
    GENERATED_BODY();

    /** The type of room */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARoomTemplate> RoomType;
    /** The type of building block */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildingBlockType BlockType;
    /** The grid location of the block */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIntPoint> BlockLocation;
};
