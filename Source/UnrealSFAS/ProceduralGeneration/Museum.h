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
	static FMapGrid GenerateVentLayout(const FMapGrid& RoomMask);	// Placeholder, as this currently does not have a way of specifying vent entrances (unless the room mask has an empty spot where the vent goes)

	/* Verification */
	static bool LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout);

	/* Placement */
	void PlaceRooms(const TArray<FRoomPlacement>& Rooms);
	void PlaceVents(const FMapGrid& VentMap);

	/* Helpers */
	static unsigned int ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	static unsigned int ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	/**
	* @brief Get a (random) room that fits within specified dimensions
	* @params Width The maximum width of the room
	* @params Depth The maximum depth of the room
	* @params OutRoom The room that was found. Can be nullptr
	* @params OutShouldBeRotated True if the room fits the dimensions, but not with its default rotation
	*/
	void GetFittingRoom(const int Width, const int Depth, UClass* OutRoom, bool& OutShouldBeRotated) const;

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The rooms to use when generating the museum"))
	TArray<TSubclassOf<ARoomTemplate>> PossibleRooms;
};
