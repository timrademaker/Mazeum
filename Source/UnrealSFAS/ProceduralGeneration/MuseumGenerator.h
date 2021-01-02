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
	 * Procedurally generate a museum layout
	 * @param PossibleRooms An array of rooms that could be placed in the museum
	 * @param OutRoomPlacement An array in which the placement of rooms for this museum layout is put
	 * @param OutHallMask A grid on which the generated halls are filled
	 * @param OutVentEntranceMask A grid on which the generated vent entrances are filled
	 * @param OutDoorMask A grid on which the generated doors are filled
	 * @param OutVentMask A grid on which the generated vent layout is filled
	 */
	static void GenerateMuseum(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutHallMask, FMapGrid& OutVentEntranceMask, FMapGrid& OutDoorMask, FMapGrid& OutVentMask);

private:
	FMuseumGenerator() = default;
	~FMuseumGenerator() = default;

	/* Generation */
	/** 
	 * Select a random museum layout
	 * @return The selected layout
	 */
	static FMapGrid SelectMuseumLayout();
	/**
	 * Determine what rooms can be placed where in order to fill up the museum
	 * @param MuseumLayout The museum layout to find rooms for
	 * @param PossibleRooms An array of rooms that could be placed in the museum
	 * @param OutRoomPlacement An array in which the placement of rooms for this museum layout is put
	 * @param OutRoomMask A grid with the locations of the found room placements filled
	 */
	static void GenerateRoomPlacement(const FMapGrid& MuseumLayout, const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutRoomMask);
	/** 
	 * Generate the layout of the vents based on vent entrances
	 * @param VentEntranceMask A grid with the locations of vent entrances filled
	 * @return A grid with the locations of the vents filled
	 */
	static FMapGrid GenerateVentLayout(const FMapGrid& VentEntranceMask);

	/* Verification */
	static bool LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout);

	/* Helpers */
	/**
	 * Find the number of consecutive empty tiles on a grid
	 * @param MuseumLayout The hall layout of the museum
	 * @param RoomMask The room mask for the museum
	 * @param X The x-coordinate from where counting should start. Should be an empty tile.
	 * @param Y The y-coordinate from where counting should start. Should be an empty tile.
	 * @param Direction The direction in which to check for empty tiles
	 * @return The number of consecutive empty tiles in a direction, starting from X and Y (including X and Y)
	 */
	static unsigned int ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	/**
	 * Find the number of consecutive tiles that have no adjacent tiles in a certain direction
	 * @param MuseumLayout The hall layout of the museum
	 * @param RoomMask The room mask for the museum
	 * @param X The x-coordinate from where counting should start. Should be a filled tile.
	 * @param Y The y-coordinate from where counting should start. Should be a filled tile.
	 * @param Direction The direction in which to check for unoccupied walls
	 * @return The number of consecutive unoccupied walls in a direction, starting from X and Y (including X and Y)
	 */
	static unsigned int ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction);
	/**
	 * Get a (random) room that fits within specified dimensions
	 * @param PossibleRooms An array of possible rooms
	 * @param Width The maximum width of the room
	 * @param Depth The maximum depth of the room
	 * @param PlacementDirection The direction in which the room should be placed
	 * @param OutRoom The room that was found. Can be nullptr
	 * @param OutRoomRotation The rotation of the room when it is placed
	 */
	static void GetFittingRoom(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, const int Width, const int Depth, const EDirection PlacementDirection, UClass*& OutRoom, FRotator& OutRoomRotation);
	/**
	 * Create a mask with all locations of a certain building block, based on the rooms in RoomPlacement
	 * @param RoomPlacement The rooms to be placed in the world
	 * @param BuildingBlockType The type of building block to find
	 * @param MaskWidth The width of the mask to place the found blocks on
	 * @param MaskDepth The depth of the mask to place the found blocks on
	 * @return A map grid with all of the locations of a certain building block set to 1
	 */
	static FMapGrid CreateBuildingBlockMask(const TArray<FRoomPlacement>& RoomPlacement, const EBuildingBlockType BuildingBlockType, int MaskWidth, int MaskDepth);

};