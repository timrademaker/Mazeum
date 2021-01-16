#include "MuseumGenerator.h"

#include "MuseumLayouts.h"

void FMuseumGenerator::GenerateMuseum(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutHallMask, FMapGrid& OutRoomMask, FMapGrid& OutVentEntranceMask, FMapGrid& OutDoorMask, FMapGrid& OutVentMask)
{
	OutHallMask = FMuseumGenerator::SelectMuseumLayout();
	const unsigned int mapWidth = OutHallMask.GetWidth();
	const unsigned int mapDepth = OutHallMask.GetDepth();

	OutRoomMask = FMapGrid(mapWidth, mapDepth);

	FMuseumGenerator::GenerateRoomPlacement(OutHallMask, PossibleRooms, OutRoomPlacement, OutRoomMask);
	
	FMuseumGenerator::SetUpTargetRoom(OutRoomPlacement);

	OutDoorMask = FMuseumGenerator::CreateBuildingBlockMask(OutRoomPlacement, EBuildingBlockType::Door, mapWidth, mapDepth);
	

	FMuseumGenerator::GenerateVentLayout(mapWidth, mapDepth, OutRoomPlacement, OutVentMask, OutVentEntranceMask);

	FMuseumGenerator::RandomizeRooms(OutRoomPlacement);
}

FMapGrid FMuseumGenerator::SelectMuseumLayout()
{
	const TArray<FMapGrid>& possibleLayouts = FMuseumLayouts::GetMuseumLayouts();

	const uint8 layoutIndex = FMath::RandHelper(possibleLayouts.Num());

	return possibleLayouts[layoutIndex];
}

void FMuseumGenerator::GenerateRoomPlacement(const FMapGrid& MuseumLayout, const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutRoomMask)
{
	OutRoomMask.Clear();

	for (unsigned int y = 0; y < MuseumLayout.GetDepth(); ++y)
	{
		for (unsigned int x = 0; x < MuseumLayout.GetWidth(); ++x)
		{
			if (!MuseumLayout.IsEmpty(x, y))
			{
				// Determine the "depth" of the emptiness
				EDirection dir = EDirection::Left;
				int emptyTiles = 0;
				emptyTiles = ContiguousEmptyTileCount(MuseumLayout, OutRoomMask, x - 1, y, dir);

				if (emptyTiles == 0)
				{
					dir = EDirection::Right;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, OutRoomMask, x + 1, y, dir);
				}

				if (emptyTiles == 0)
				{
					dir = EDirection::Up;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, OutRoomMask, x, y - 1, dir);
				}

				if (emptyTiles == 0)
				{
					dir = EDirection::Down;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, OutRoomMask, x, y + 1, dir);
				}
				// TODO: Issue: Not all directions are checked if one of them has some space but not enough to place a room

				if (emptyTiles == 0)
				{
					continue;
				}

				// Determine the number of walls with adjacent empty space
				// Variables representing the dimensions of the empty space
				unsigned int emptyDepth = 0;
				unsigned int emptyWidth = 0;

				if (dir == EDirection::Left || dir == EDirection::Right)
				{
					emptyWidth = emptyTiles;
					emptyDepth = ContiguousUnoccupiedWallCount(MuseumLayout, OutRoomMask, x, y, dir);
				}
				else
				{
					emptyDepth = emptyTiles;
					emptyWidth = ContiguousUnoccupiedWallCount(MuseumLayout, OutRoomMask, x, y, dir);
				}

				// Find a suitable room for these dimensions
				UClass* room = nullptr;
				FRotator roomRotation;
				GetFittingRoom(PossibleRooms, emptyWidth, emptyDepth, dir, room, roomRotation);

				if (room)
				{
					FRoomPlacement placement;
					placement.RoomType = room;
					placement.Direction = dir;
					placement.Rotation = roomRotation;


					const FIntVector roomSize = Cast<ARoomTemplate>(room->GetDefaultObject())->RoomSize;
					const unsigned int roomX = (roomRotation.IsNearlyZero() || roomRotation.Equals(FRotator(0.0f, 180.0f, 0.0f))) ? roomSize.X : roomSize.Y;
					const unsigned int roomY = (roomRotation.IsNearlyZero() || roomRotation.Equals(FRotator(0.0f, 180.0f, 0.0f))) ? roomSize.Y : roomSize.X;

					// Fill RoomMask (based on roomShouldBeRotated and dir)
					//  Remember: Current x, y is a wall
					if (dir == EDirection::Left)
					{
						// Fill X to X - depth
						// Fill Y to Y + width

						for (unsigned int i = x - roomX; i < x; ++i)
						{
							for (unsigned int j = y; j < y + roomY; ++j)
							{
								OutRoomMask.Set(i, j, true);
							}
						}

						placement.Position = FIntVector(x - 1, y, 0);
					}
					else if (dir == EDirection::Right)
					{
						// Fill X to X + depth
						// Fill Y to Y + width

						for (unsigned int i = x + 1; i <= x + roomX; ++i)
						{
							for (unsigned int j = y; j < y + roomY; ++j)
							{
								OutRoomMask.Set(i, j, true);
							}
						}

						placement.Position = FIntVector(x + 1, y, 0);
					}
					else if (dir == EDirection::Up)
					{
						// Fill X to X + width
						// Fill Y to Y - depth

						for (unsigned int i = x; i < x + roomX; ++i)
						{
							for (unsigned int j = y - roomY; j < y; ++j)
							{
								OutRoomMask.Set(i, j, true);
							}
						}

						placement.Position = FIntVector(x, y - 1, 0);
					}
					else if (dir == EDirection::Down)
					{
						// Fill X to X + width
						// Fill Y to Y + depth

						for (unsigned int i = x; i < x + roomX; ++i)
						{
							for (unsigned int j = y + 1; j <= y + roomY; ++j)
							{
								OutRoomMask.Set(i, j, true);
							}
						}

						placement.Position = FIntVector(x, y + 1, 0);
					}

					OutRoomPlacement.Add(placement);

					// Go over the same tile again, as there might be another free tile in a different direction
					x -= 1;
				}
			}
		}
	}
}

void FMuseumGenerator::GenerateVentLayout(const uint8 MaskWidth, const uint8 MaskDepth, const TArray<FRoomPlacement>& RoomPlacement, FMapGrid& OutVentMask, FMapGrid& OutVentEntranceMask)
{
	OutVentEntranceMask = FMuseumGenerator::CreateBuildingBlockMask(RoomPlacement, EBuildingBlockType::Vent, MaskWidth, MaskDepth);
	OutVentMask = FMapGrid(MaskWidth, MaskDepth);

	TArray<FIntPoint> allVentEntrances;

	// Find the vent that all rooms should connect to
	FIntPoint connectorRoomVentLocation;

	for (const FRoomPlacement& room : RoomPlacement)
	{
		if (!room.LasersAreEnabled)
		{
			TArray<FIntPoint> ventEntrances;
			FindBuildingBlockLocations(room, EBuildingBlockType::Vent, ventEntrances);
			
			check(ventEntrances.Num() > 0 && "Every room should have a vent entrance. If they do, the vent couldn't be found for some reason.");

			// Just take the first entrance, regardless of how many there are
			connectorRoomVentLocation = ventEntrances[0];

			break;
		}
	}

	// Connect all vents with the connecting room
	for (uint8 x = 0; x < MaskWidth; ++x)
	{
		for (uint8 y = 0; y < MaskDepth; ++y)
		{
			if (!OutVentEntranceMask.IsEmpty(x, y))
			{
				FIntPoint thisVentLocation(x, y);
				ConnectVents(thisVentLocation, connectorRoomVentLocation, OutVentMask);

				allVentEntrances.Add(thisVentLocation);
			}
		}
	}

	// Pick some extra vents to connect, in an attempt to make the game a bit more interesting
	const uint8 extraConnections = FMath::RandRange(allVentEntrances.Num() / 4, allVentEntrances.Num());

	for (uint8 i = 0; i < extraConnections; ++i)
	{
		// Pick random pairs of vents to connect
		const uint8 fromVentIndex = FMath::RandHelper(allVentEntrances.Num());
		uint8 toVentIndex;
		do
		{
			toVentIndex = FMath::RandHelper(allVentEntrances.Num());
		} while (toVentIndex == fromVentIndex);

		ConnectVents(allVentEntrances[fromVentIndex], allVentEntrances[toVentIndex], OutVentMask);
	}
}

void FMuseumGenerator::ConnectVents(const FIntPoint& FromVent, const FIntPoint& ToVent, FMapGrid& VentMask)
{
	// Determine in which direction the connection should be placed (i.e. positive or negative direction)
	const FIntPoint direction(
		FromVent.X < ToVent.X ? 1 : -1,
		FromVent.Y < ToVent.Y ? 1 : -1
	);

	// Pick a random direction to connect first (0 means X, 1 means Y)
	uint8 indexToConnect = FMath::RandBool() ? 1 : 0;

	FIntPoint currentPosition = FromVent;

	for (uint8 i = 0; i < 2; ++i)
	{
		while (currentPosition[indexToConnect] != ToVent[indexToConnect])
		{
			VentMask.Set(currentPosition.X, currentPosition.Y, true);
			
			currentPosition[indexToConnect] += direction[indexToConnect];
		}

		// Flip the index
		indexToConnect = ~indexToConnect & 1;
	}

	VentMask.Set(ToVent.X, ToVent.Y, true);
}

void FMuseumGenerator::SetUpTargetRoom(TArray<FRoomPlacement>& RoomPlacement)
{
	check(RoomPlacement.Num() > 1 && "Can't create anything that plays like a game if there's just a single room");

	const uint8 targetRoomIndex = FMath::RandHelper(RoomPlacement.Num());
	uint8 connectorRoomIndex;

	do
	{
		connectorRoomIndex = FMath::RandHelper(RoomPlacement.Num());
	} while (connectorRoomIndex == targetRoomIndex);

	RoomPlacement[targetRoomIndex].RoomContainsTargetItem = true;
	RoomPlacement[connectorRoomIndex].LasersAreEnabled = false;
}

void FMuseumGenerator::RandomizeRooms(TArray<FRoomPlacement>& RoomPlacement, const uint8 MaximumLaserDisablePercentage)
{
	const uint8 numLasersToDisable = FMath::RandRange(0, (MaximumLaserDisablePercentage * RoomPlacement.Num()) / 100);
	
	// Get all room indices that can be chosen from
	TArray<uint8> roomIndices;
	roomIndices.Reserve(RoomPlacement.Num() - 1);
	for (uint8 i = 0; i < RoomPlacement.Num(); ++i)
	{
		if (!RoomPlacement[i].RoomContainsTargetItem)
		{
			roomIndices.Add(i);
		}
	}

	for (uint8 i = 0; i < numLasersToDisable; ++i)
	{
		// Pick the index of a room index in roomIndices
		const uint8 roomIndexIndex = FMath::RandHelper(roomIndices.Num());
		// Get the associated room index
		const uint8 disableLasersInRoomWithIndex = roomIndices[roomIndexIndex];
		// Disable lasers in the chosen room
		RoomPlacement[disableLasersInRoomWithIndex].LasersAreEnabled = false;

		// Make sure the room isn't picked again
		roomIndices.RemoveAt(roomIndexIndex);
	}
}

unsigned int FMuseumGenerator::ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection SearchDirection)
{
	int emptyTiles = 0;

	int maxX = MuseumLayout.GetWidth() - 1;
	int maxY = MuseumLayout.GetDepth() - 1;

	while (X >= 0 && X <= maxX
		&& Y >= 0 && Y <= maxY
		&& MuseumLayout.IsEmpty(X, Y)
		&& RoomMask.IsEmpty(X, Y)
		)
	{
		++emptyTiles;

		if (SearchDirection == EDirection::Left)
		{
			--X;
		}
		else if (SearchDirection == EDirection::Right)
		{
			++X;
		}
		else if (SearchDirection == EDirection::Up)
		{
			--Y;
		}
		else
		{
			++Y;
		}
	}

	return emptyTiles;
}

unsigned int FMuseumGenerator::ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection EmptyTileDirection)
{
	// The offset to move by with each step (e.g. (1, 0) means we are looking for unoccupied walls to the right)
	FIntPoint searchOffset(1, 0);

	if (EmptyTileDirection == EDirection::Left || EmptyTileDirection == EDirection::Right)
	{
		//searchDirection = EDirection::Down;
		searchOffset = FIntPoint(0, 1);
	}

	// The offset in which to check for walls (e.g. (-1, 0) means that we are looking for blocking walls on the left of X, Y)
	FIntPoint wallCheckOffset;

	if (EmptyTileDirection == EDirection::Left)
	{
		wallCheckOffset = FIntPoint(-1, 0);
	}
	else if (EmptyTileDirection == EDirection::Right)
	{
		wallCheckOffset = FIntPoint(1, 0);
	}
	else if (EmptyTileDirection == EDirection::Up)
	{
		wallCheckOffset = FIntPoint(0, -1);
	}
	else
	{
		wallCheckOffset = FIntPoint(0, 1);
	}

	int unoccupiedWallCount = 0;

	const int maxX = MuseumLayout.GetWidth() - 1;
	const int maxY = MuseumLayout.GetDepth() - 1;

	while (X >= 0 && X <= maxX
		&& Y >= 0 && Y <= maxY
		&& !MuseumLayout.IsEmpty(X, Y)
		&& MuseumLayout.IsEmpty(X + wallCheckOffset.X, Y + wallCheckOffset.Y)	// Check if there is a wall perpendicular to the wall we are checking
		&& (RoomMask.IsEmpty(X - 1, Y) || RoomMask.IsEmpty(X + 1, Y) || RoomMask.IsEmpty(X, Y - 1) || RoomMask.IsEmpty(X, Y + 1))	// Check if there is a room next to the wall we are checking
		)
	{
		++unoccupiedWallCount;

		X += searchOffset.X;
		Y += searchOffset.Y;
	}

	return unoccupiedWallCount;
}

void FMuseumGenerator::GetFittingRoom(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, const int Width, const int Depth, const EDirection PlacementDirection, UClass*& OutRoom, FRotator& OutRoomRotation)
{
	if (Width == 0 || Depth == 0)
	{
		return;
	}

	OutRoom = nullptr;
	OutRoomRotation = FRotator(0.0f, 0.0f, 0.0f);

	UClass* possibleRoom = nullptr;
	bool possibleRoomShouldBeRotated = false;
	bool possibleRoomCanBeRotated = false;
	bool hasFoundRoom = false;

	TArray<FIntPoint> roomDoors;

	// Shuffle the rooms so we don't always end up picking the same ones
	TArray<TSubclassOf<ARoomTemplate>> shuffledRoomTemplates = PossibleRooms;
	const int32 lastArrayIndex = shuffledRoomTemplates.Num() - 1;
	for (int32 i = 0; i <= lastArrayIndex; ++i)
	{
		int32 swapIndex = FMath::RandRange(i, lastArrayIndex);
		if (swapIndex != i)
		{
			shuffledRoomTemplates.Swap(i, swapIndex);
		}
	}

	// Go over all rooms and see if a fitting room can be found
	for (size_t i = 0; i < shuffledRoomTemplates.Num() && !hasFoundRoom; ++i)
	{
		ARoomTemplate* room = Cast<ARoomTemplate>(shuffledRoomTemplates[i]->GetDefaultObject());
		auto size = room->RoomSize;
		if (size.X <= Width && size.Y <= Depth)
		{
			possibleRoom = shuffledRoomTemplates[i];
			possibleRoomShouldBeRotated = false;
			possibleRoomCanBeRotated = false;
		}

		if (size.X <= Depth && size.Y <= Width)
		{
			if (!possibleRoom)
			{
				possibleRoom = shuffledRoomTemplates[i];
				possibleRoomShouldBeRotated = true;
			}

			possibleRoomCanBeRotated = true;
		}

		if (possibleRoom)
		{
			roomDoors.Empty();
			room->GetLocationsOfBlocksWithType(EBuildingBlockType::Door, possibleRoom, roomDoors);

			// Check left if:    Up/Down && rotated || dir == right && !rotated
			// Check right if:   Up/Down && rotated || dir == left  && !rotated
			// Check up if:   Left/Right && rotated || dir == down  && !rotated
			// Check down if: Left/Right && rotated || dir == up    && !rotated

			if ((PlacementDirection == EDirection::Left && !possibleRoomShouldBeRotated)
				|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Up || PlacementDirection == EDirection::Down)))
			{
				for (const auto& door : roomDoors)
				{
					// Check the right wall for doors
					if (door.X == size.X - 1)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Up)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Down)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}

						break;
					}
					// Check the left wall for doors
					else if (door.X == 0)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Up)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Down)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else
						{
							OutRoomRotation = FRotator(0.0f, 180.0f, 0.0f);
						}

						break;
					}
				}
			}

			if (!hasFoundRoom
				&& ((PlacementDirection == EDirection::Right && !possibleRoomShouldBeRotated)
					|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Up || PlacementDirection == EDirection::Down))))
			{
				for (const auto& door : roomDoors)
				{
					// Check the left wall for doors
					if (door.X == 0)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Up)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Down)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}

						break;
					}
					// Check the right wall for doors
					else if (door.X == size.X - 1)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Up)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Down)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else
						{
							OutRoomRotation = FRotator(0.0f, 180.0f, 0.0f);
						}

						break;
					}
				}
			}

			if (!hasFoundRoom
				&& ((PlacementDirection == EDirection::Up && !possibleRoomShouldBeRotated)
					|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right))))
			{
				for (const auto& door : roomDoors)
				{
					// Check the bottom wall for doors
					if (door.Y == size.Y - 1)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Left)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Right)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}

						break;
					}
					// Check the top wall for doors
					else if (door.Y == 0)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Left)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Right)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else
						{
							OutRoomRotation = FRotator(0.0f, 180.0f, 0.0f);
						}

						break;
					}
				}
			}

			if (!hasFoundRoom
				&& ((PlacementDirection == EDirection::Down && !possibleRoomShouldBeRotated)
					|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right))))
			{
				for (const auto& door : roomDoors)
				{
					// Check the top wall for doors
					if (door.Y == 0)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Left)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Right)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}

						break;
					}
					// Check the bottom wall for doors
					else if (door.Y == size.Y - 1)
					{
						hasFoundRoom = true;

						if (PlacementDirection == EDirection::Left)
						{
							OutRoomRotation = FRotator(0.0f, -90.0f, 0.0f);
						}
						else if (PlacementDirection == EDirection::Right)
						{
							OutRoomRotation = FRotator(0.0f, 90.0f, 0.0f);
						}
						else
						{
							OutRoomRotation = FRotator(0.0f, 180.0f, 0.0f);
						}

						break;
					}
				}
			}

			if (!hasFoundRoom)
			{
				possibleRoom = nullptr;
			}
			else
			{
				OutRoom = possibleRoom;
			}
		}
	}
}

FMapGrid FMuseumGenerator::CreateBuildingBlockMask(const TArray<FRoomPlacement>& RoomPlacement, const EBuildingBlockType BuildingBlockType, int MaskWidth, int MaskDepth)
{
	FMapGrid blockMask(MaskWidth, MaskDepth);

	TArray<FIntPoint> blockLocations;

	for (const auto& room : RoomPlacement)
	{
		FindBuildingBlockLocations(room, BuildingBlockType, blockLocations);

		for (const FIntPoint& blockPos : blockLocations)
		{
			blockMask.Set(blockPos.X, blockPos.Y, true);
		}
	}

	return blockMask;
}

void FMuseumGenerator::FindBuildingBlockLocations(const FRoomPlacement& RoomPlacement, const EBuildingBlockType BuildingBlockType, TArray<FIntPoint>& OutFoundBuildingBlockLocations)
{
	OutFoundBuildingBlockLocations.Empty();

	FIntPoint roomLeftTop(RoomPlacement.Position.X, RoomPlacement.Position.Y);

	const bool clockwiseRotation = FMath::IsNearlyEqual(RoomPlacement.Rotation.Yaw, 90.0f);
	const bool counterclockwiseRotation = FMath::IsNearlyEqual(RoomPlacement.Rotation.Yaw, -90.0f);
	const bool upsideDownRotation = FMath::IsNearlyEqual(RoomPlacement.Rotation.Yaw, 180.0f);

	const ARoomTemplate* roomTemplate = RoomPlacement.RoomType->GetDefaultObject<ARoomTemplate>();

	// Rotate the room if needed
	if (RoomPlacement.Direction == EDirection::Left)
	{
		roomLeftTop.X -= (clockwiseRotation || counterclockwiseRotation) ? roomTemplate->RoomSize.Y : roomTemplate->RoomSize.X;
		roomLeftTop.X += 1;
	}
	else if (RoomPlacement.Direction == EDirection::Up)
	{
		roomLeftTop.Y -= (clockwiseRotation || counterclockwiseRotation) ? roomTemplate->RoomSize.X : roomTemplate->RoomSize.Y;
		roomLeftTop.Y += 1;
	}

	TArray<FIntPoint> blockLocations;
	roomTemplate->GetLocationsOfBlocksWithType(BuildingBlockType, RoomPlacement.RoomType, blockLocations);

	// Determine coordinates of each building block, taking the room's rotation into account
	for (const FIntPoint& blockPos : blockLocations)
	{
		int blockX = 0;
		int blockY = 0;

		if (clockwiseRotation)
		{
			blockX = roomTemplate->RoomSize.Y - 1 - blockPos.Y;
			blockY = blockPos.X;
		}
		else if (upsideDownRotation)
		{
			blockX = roomTemplate->RoomSize.X - 1 - blockPos.X;
			blockY = roomTemplate->RoomSize.Y - 1 - blockPos.Y;
		}
		else if (counterclockwiseRotation)
		{
			blockX = blockPos.Y;
			blockY = roomTemplate->RoomSize.X - 1 - blockPos.X;
		}
		else
		{
			blockX = blockPos.X;
			blockY = blockPos.Y;
		}

		OutFoundBuildingBlockLocations.Add(FIntPoint(blockX + roomLeftTop.X, blockY + roomLeftTop.Y));
	}
}
