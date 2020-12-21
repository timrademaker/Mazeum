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
	void GenerateRoomPlacement(const FMapGrid& MuseumLayout, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& RoomMask) const;
	static FMapGrid GenerateVentLayout(const FMapGrid& VentEntranceMask);

	/* Verification */
	static bool LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout);

	/* Placement */
	void PlaceHalls(const FMapGrid& MuseumLayout);
	void PlaceRooms(const TArray<FRoomPlacement>& Rooms);
	void PlaceVents(const FMapGrid& VentMap);

	/* Helpers */
	static unsigned int ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	static unsigned int ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	/**
	* @brief Get a (random) room that fits within specified dimensions
	* @params Width The maximum width of the room
	* @params Depth The maximum depth of the room
	* @params PlacementDirection The direction in which the room should be placed
	* @params OutRoom The room that was found. Can be nullptr
	* @params OutRoomRotation The rotation of the room when it is placed
	*/
	void GetFittingRoom(const int Width, const int Depth, const EDirection PlacementDirection, UClass*& OutRoom, FRotator& OutRoomRotation) const;
	/**
	* @brief Create a mask with all vent entrances from the rooms in RoomPlacement
	* @params RoomPlacement The rooms to get the vents from
	* @returns A map grid with all of the room's vent entrances
	*/
	static FMapGrid CreateVentEntranceMask(const TArray<FRoomPlacement>& RoomPlacement, int MaskWidth, int MaskDepth);

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The rooms to use when generating the museum"))
	TArray<TSubclassOf<ARoomTemplate>> PossibleRooms;

	// TODO: Remove as this is temporary
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> HallBlock;
};
