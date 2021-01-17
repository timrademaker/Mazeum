// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../RoomTemplate.h"

/** A struct representing how a room should be placed */
struct UNREALSFAS_API FRoomPlacement
{
    /** The room's class */
    TSubclassOf<ARoomTemplate> RoomType;
    /** The grid position this room should be placed on */
    FIntVector Position;
    /** The rotation of the room */
    FRotator Rotation;
    /** The direction in which the room is to be placed, relative to Position */
    enum class EDirection Direction;
    /** If true, lasers in this room should be enabled */
    bool LasersAreEnabled = true;
    /** If true, this room contains the item the player needs to pick up in order to win */
    bool RoomContainsTargetItem = false;
};
