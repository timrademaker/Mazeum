// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../MapGrid.h"
#include "RoomPlacement.h"

#include "Museum.generated.h"

enum class EDirection
{
	Left,
	Right,
	Up,
	Down
};

UCLASS()
class UNREALSFAS_API AMuseum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuseum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/* Generation */
	static FMapGrid SelectMuseumLayout();
	static void GenerateRoomPlacement(const FMapGrid& MuseumLayout, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& RoomMask);
	static FMapGrid GenerateVentLayout(const FMapGrid& RoomMask);	// Placeholder, as this currently does not have a way of specifying vent entrances (unless the room mask has an empty spot where the vent goes)

	/* Verification */
	static bool LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout);

	/* Placement */
	void PlaceRooms(const TArray<FRoomPlacement>& Rooms);
	void PlaceVents(const FMapGrid& VentMap);

	/* Helpers */
	static unsigned int ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	static unsigned int ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
};
