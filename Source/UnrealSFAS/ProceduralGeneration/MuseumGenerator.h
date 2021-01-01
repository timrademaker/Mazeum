#pragma once

#include "CoreMinimal.h"

#include "../MapGrid.h"
#include "RoomPlacement.h"

enum class EDirection
{
	Left,
	Right,
	Up,
	Down
};

class FMuseumGenerator
{
public:
	/**
	 *
	 */
	static void GenerateMuseum(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutHallMask, FMapGrid& OutRoomMask, FMapGrid& OutDoorMask, FMapGrid& OutVentMask);

private:
	FMuseumGenerator() = default;
	~FMuseumGenerator() = default;

	/* Generation */
	static FMapGrid SelectMuseumLayout();
	static void GenerateRoomPlacement(const FMapGrid& MuseumLayout, const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutRoomMask);
	static FMapGrid GenerateVentLayout(const FMapGrid& VentEntranceMask);

	/* Verification */
	static bool LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout);

	/* Helpers */
	static unsigned int ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	static unsigned int ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	/**
	* Get a (random) room that fits within specified dimensions
	* @param Width The maximum width of the room
	* @param Depth The maximum depth of the room
	* @param PlacementDirection The direction in which the room should be placed
	* @param OutRoom The room that was found. Can be nullptr
	* @param OutRoomRotation The rotation of the room when it is placed
	*/
	static void GetFittingRoom(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, const int Width, const int Depth, const EDirection PlacementDirection, UClass*& OutRoom, FRotator& OutRoomRotation);
	/**
	* Create a mask with all vent entrances from the rooms in RoomPlacement
	* @param RoomPlacement The rooms to get the vents from
	* @return A map grid with all of the room's vent entrances
	*/
	static FMapGrid CreateVentEntranceMask(const TArray<FRoomPlacement>& RoomPlacement, int MaskWidth, int MaskDepth);

};