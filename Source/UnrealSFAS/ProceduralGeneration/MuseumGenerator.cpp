#include "MuseumGenerator.h"

void FMuseumGenerator::GenerateMuseum(const TArray<TSubclassOf<ARoomTemplate>>& PossibleRooms, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& OutHallMask, FMapGrid& OutRoomMask, FMapGrid& OutDoorMask, FMapGrid& OutVentMask)
{
	// TODO: Is room mask needed as arg? Would OutVentEntranceMask not be more useful?

	OutHallMask = FMuseumGenerator::SelectMuseumLayout();
	FMuseumGenerator::GenerateRoomPlacement(OutHallMask, PossibleRooms, OutRoomPlacement, OutRoomMask);
	FMapGrid ventEntranceMask = FMuseumGenerator::CreateVentEntranceMask(OutRoomPlacement, OutHallMask.GetWidth(), OutHallMask.GetDepth());
	
	/*
	OutVentMask = FMuseumGenerator::GenerateVentLayout(ventEntranceMask);
	// TODO: Door mask
	// TODO: Verify layout

	while (!FMuseumGenerator::LayoutIsValid(OutHallMask, OutRoomMask, OutVentMask))
	{
		OutVentMask = GenerateVentLayout(OutRoomMask);
	}
	*/
}

FMapGrid FMuseumGenerator::SelectMuseumLayout()
{
    //// TODO: Implement properly

	FMapGrid grid(17, 15);
	grid.SetRow(5, 0b111111110000);
	grid.SetRow(6, 0b100000010000);
	grid.SetRow(7, 0b100000010000);
	grid.SetRow(8, 0b100000010000);
	grid.SetRow(9, 0b111111110000);
	return grid;
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
					emptyDepth = ContiguousUnoccupiedWallCount(MuseumLayout, OutRoomMask, x, y, EDirection::Down);

				}
				else
				{
					emptyDepth = emptyTiles;
					emptyWidth = ContiguousUnoccupiedWallCount(MuseumLayout, OutRoomMask, x, y, EDirection::Right);
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

FMapGrid FMuseumGenerator::GenerateVentLayout(const FMapGrid& VentEntranceMask)
{
	// TODO: Implement
	check(false);
	return FMapGrid();
}

bool FMuseumGenerator::LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout)
{
	// TODO: Implement
	check(false);
	return true;
}

unsigned int FMuseumGenerator::ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction)
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

		if (Direction == EDirection::Left)
		{
			--X;
		}
		else if (Direction == EDirection::Right)
		{
			++X;
		}
		else if (Direction == EDirection::Up)
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

unsigned int FMuseumGenerator::ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction)
{
	int unoccupiedWallCount = 0;

	const int maxX = MuseumLayout.GetWidth() - 1;
	const int maxY = MuseumLayout.GetDepth() - 1;

	while (X >= 0 && X <= maxX
		&& Y >= 0 && Y <= maxY
		&& !MuseumLayout.IsEmpty(X, Y)
		&& (RoomMask.IsEmpty(X - 1, Y) || RoomMask.IsEmpty(X + 1, Y) || RoomMask.IsEmpty(X, Y - 1) || RoomMask.IsEmpty(X, Y + 1))
		)
	{
		++unoccupiedWallCount;

		if (Direction == EDirection::Left)
		{
			--X;
		}
		else if (Direction == EDirection::Right)
		{
			++X;
		}
		else if (Direction == EDirection::Up)
		{
			--Y;
		}
		else
		{
			++Y;
		}
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

	for (size_t i = 0; i < PossibleRooms.Num() && !hasFoundRoom; ++i)
	{
		auto size = Cast<ARoomTemplate>(PossibleRooms[i]->GetDefaultObject())->RoomSize;
		if (size.X <= Width && size.Y <= Depth)
		{
			possibleRoom = PossibleRooms[i];
			possibleRoomShouldBeRotated = false;
			possibleRoomCanBeRotated = false;
		}

		if (size.X <= Depth && size.Y <= Width)
		{
			if (!possibleRoom)
			{
				possibleRoom = PossibleRooms[i];
				possibleRoomShouldBeRotated = true;
			}

			possibleRoomCanBeRotated = true;
		}

		if (possibleRoom)
		{
			roomDoors.Empty();
			Cast<ARoomTemplate>(possibleRoom)->GetLocationsOfBlocksWithType(EBuildingBlockType::Door, possibleRoom, roomDoors);

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
				&& (PlacementDirection == EDirection::Right && !possibleRoomShouldBeRotated)
				|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Up || PlacementDirection == EDirection::Down)))
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
				&& (PlacementDirection == EDirection::Up && !possibleRoomShouldBeRotated)
				|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right)))
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
				&& (PlacementDirection == EDirection::Down && !possibleRoomShouldBeRotated)
				|| ((possibleRoomShouldBeRotated || possibleRoomCanBeRotated) && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right)))
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

FMapGrid FMuseumGenerator::CreateVentEntranceMask(const TArray<FRoomPlacement>& RoomPlacement, int MaskWidth, int MaskDepth)
{
	FMapGrid ventEntrances(MaskWidth, MaskDepth);

	TArray<FIntPoint> ventLocations;


	bool clockwiseRotation = false;
	bool counterclockwiseRotation = false;
	bool upsideDownRotation = false;

	for (const auto& room : RoomPlacement)
	{
		clockwiseRotation = FMath::IsNearlyEqual(room.Rotation.Yaw, 90.0f);
		counterclockwiseRotation = FMath::IsNearlyEqual(room.Rotation.Yaw, -90.0f);
		upsideDownRotation = FMath::IsNearlyEqual(room.Rotation.Yaw, 180.0f);

		const ARoomTemplate* roomTemplate = room.RoomType->GetDefaultObject<ARoomTemplate>();

		FIntPoint roomLeftTop(room.Position.X, room.Position.Y);
		if (room.Direction == EDirection::Left)
		{
			roomLeftTop.X -= (clockwiseRotation || counterclockwiseRotation) ? roomTemplate->RoomSize.Y : roomTemplate->RoomSize.X;
			roomLeftTop.X += 1;
		}
		else if (room.Direction == EDirection::Up)
		{
			roomLeftTop.Y -= (clockwiseRotation || counterclockwiseRotation) ? roomTemplate->RoomSize.X : roomTemplate->RoomSize.Y;
			roomLeftTop.Y += 1;
		}

		ventLocations.Empty();
		roomTemplate->GetLocationsOfBlocksWithType(EBuildingBlockType::Vent, room.RoomType, ventLocations);

		for (const auto& ventPos : ventLocations)
		{
			int ventX = 0;
			int ventY = 0;

			if (clockwiseRotation)
			{
				ventX = roomTemplate->RoomSize.Y - 1 - ventPos.Y;
				ventY = ventPos.X;
			}
			else if (upsideDownRotation)
			{
				ventX = roomTemplate->RoomSize.X - 1 - ventPos.X;
				ventY = roomTemplate->RoomSize.Y - 1 - ventPos.Y;
			}
			else if (counterclockwiseRotation)
			{
				ventX = ventPos.Y;
				ventY = roomTemplate->RoomSize.X - 1 - ventPos.X;
			}
			else
			{
				ventX = ventPos.X;
				ventY = ventPos.Y;
			}

			ventEntrances.Set(ventX + roomLeftTop.X, ventY + roomLeftTop.Y, true);
		}
	}

	return ventEntrances;
}
