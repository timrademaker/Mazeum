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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ARoomTemplate> RoomType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildingBlockType BlockType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIntPoint> BlockLocation;
};
