// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../RoomTemplate.h"

/**
 * 
 */
struct UNREALSFAS_API FRoomPlacement
{
    TSubclassOf<ARoomTemplate> RoomType;
    FIntVector Position;
    FRotator Rotation;
    EDirection Direction;
};
