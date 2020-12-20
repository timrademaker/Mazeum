// Fill out your copyright notice in the Description page of Project Settings.


#include "Museum.h"

#include "../ConstantsFunctionLibrary.h"


// Sets default values
AMuseum::AMuseum()
{
 	// No need to tick
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMuseum::BeginPlay()
{
	Super::BeginPlay();

	// Generate museum
	FMapGrid museumLayout = AMuseum::SelectMuseumLayout();
	TArray<FRoomPlacement> rooms;
	FMapGrid roomMask(10, 10);
	GenerateRoomPlacement(museumLayout, rooms, roomMask);

	/*
	FMapGrid ventLayout = GenerateVentLayout(roomMask);

	// Verify layout
	while (!AMuseum::LayoutIsValid(museumLayout, roomMask, ventLayout))
	{
		ventLayout = GenerateVentLayout(roomMask);
	}

	// Create map
	PlaceRooms(rooms);
	PlaceVents(ventLayout);
	*/
}

FMapGrid AMuseum::SelectMuseumLayout()
{
	//// TODO: Implement properly

	FMapGrid grid(10, 10);
	grid.SetRow(3, 0b1111111000);
	grid.SetRow(4, 0b1100001000);
	grid.SetRow(5, 0b1100001000);
	grid.SetRow(6, 0b1111111000);

	return grid;
}

void AMuseum::GenerateRoomPlacement(const FMapGrid& MuseumLayout, TArray<FRoomPlacement>& OutRoomPlacement, FMapGrid& RoomMask) const
{
	RoomMask.Clear();

	for (unsigned int y = 0; y < MuseumLayout.GetDepth(); ++y)
	{
		for (unsigned int x = 0; x < MuseumLayout.GetWidth(); ++x)
		{
			if (!MuseumLayout.IsEmpty(x, y))
			{
				// Determine the "depth" of the emptiness
				EDirection dir = EDirection::Left;
				int emptyTiles = 0;
				emptyTiles = ContiguousEmptyTileCount(MuseumLayout, RoomMask, x - 1, y, dir);

				if (emptyTiles == 0)
				{
					dir = EDirection::Right;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, RoomMask, x + 1, y, dir);
				}

				if (emptyTiles == 0)
				{
					dir = EDirection::Up;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, RoomMask, x, y - 1, dir);
				}

				if (emptyTiles == 0)
				{
					dir = EDirection::Down;
					emptyTiles = ContiguousEmptyTileCount(MuseumLayout, RoomMask, x, y + 1, dir);
				}

				if (emptyTiles == 0)
				{
					continue;
				}
				// TODO: Potential issue: This could leave out corner tiles in some directions if there's empty space above the tile, but not enough to place a room


				// Determine the number of walls with adjacent empty space
				// Variables representing the dimensions of the empty space
				unsigned int emptyDepth = emptyTiles;
				unsigned int emptyWidth = 0;

				if (dir == EDirection::Left || dir == EDirection::Right)
				{
					emptyWidth = ContiguousUnoccupiedWallCount(MuseumLayout, RoomMask, x, y, EDirection::Down);

				}
				else
				{
					emptyWidth = ContiguousUnoccupiedWallCount(MuseumLayout, RoomMask, x, y, EDirection::Right);
				}

				// Find a suitable room for these dimensions
				UClass* room = nullptr;
				FRotator roomRotation;
				GetFittingRoom(emptyWidth, emptyDepth, dir, room, roomRotation);

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

						for (unsigned int i = x - roomX; i < x; --i)
						{
							for (unsigned int j = y; j < y + roomY; ++j)
							{
								RoomMask.Set(i, j, true);
							}
						}

						placement.Position = FIntVector(x - 1, y, 0);
					}
					else if (dir == EDirection::Right)
					{
						// Fill X to X + depth
						// Fill Y to Y + width

						for (unsigned int i = x + 1; i < x + roomX; ++i)
						{
							for (unsigned int j = y; j < y + roomY; ++j)
							{
								RoomMask.Set(i, j, true);
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
								RoomMask.Set(i, j, true);
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
							for (unsigned int j = y + 1; j < y + roomY; ++j)
							{
								RoomMask.Set(i, j, true);
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

FMapGrid AMuseum::GenerateVentLayout(const FMapGrid& RoomMask)
{
	// TODO: Implement
	check(false);
	return FMapGrid();
}

bool AMuseum::LayoutIsValid(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, const FMapGrid& VentLayout)
{
	// TODO: Implement
	check(false);
	return true;
}

void AMuseum::PlaceHalls(const FMapGrid& MuseumLayout)
{
	if (!HallBlock)
	{
		return;
	}

	const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
	const FVector museumLocation = GetActorLocation();

	for (unsigned int x = 0; x < MuseumLayout.GetWidth(); ++x)
	{
		for (unsigned int y = 0; y < MuseumLayout.GetDepth(); ++y)
		{
			if (!MuseumLayout.IsEmpty(x, y))
			{
				FVector loc = FVector(x * blockSize.X, y * blockSize.Y, 0.0f) + museumLocation;

				AActor* block = GetWorld()->SpawnActor<AActor>(HallBlock, loc, FRotator());

			}
		}
	}
}

void AMuseum::PlaceRooms(const TArray<FRoomPlacement>& Rooms)
{
	const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
	const FVector museumLocation = GetActorLocation();

	for (const auto& room : Rooms)
	{
		FVector roomLocation = museumLocation + FVector(room.Position.X, room.Position.Y, 0.0f) * blockSize;
		const FVector realRoomSize = FVector(room.RoomType->GetDefaultObject<ARoomTemplate>()->RoomSize) * blockSize;

		// Adjust spawning location, as the tile a room is supposed to spawn on is "incorrect" for Up and Left
		if (room.Direction == EDirection::Up)
		{
			roomLocation.Y -= (realRoomSize.Y - blockSize.Y);
		}
		else if (room.Direction == EDirection::Left)
		{
			roomLocation.X -= (realRoomSize.X - blockSize.X);
		}

		// Spawn actor (temporary solution)
		ARoomTemplate* r = GetWorld()->SpawnActor<ARoomTemplate>(room.RoomType, roomLocation, FRotator());
		r->SetRoomRotation(room.Rotation);
	}
}

void AMuseum::PlaceVents(const FMapGrid& VentMap)
{
	// TODO: Implement
	check(false);
}

unsigned int AMuseum::ContiguousEmptyTileCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction)
{
	int emptyTiles = 0;

	int maxX = MuseumLayout.GetWidth() - 1;
	int maxY = MuseumLayout.GetDepth() - 1;

	bool roomEmpty = RoomMask.IsEmpty(X, Y);

	while (X >= 0 && X <= maxX
		&& Y >= 0 && Y <= maxY
		&& MuseumLayout.IsEmpty(X, Y)
		&& roomEmpty
		)
	{
		++emptyTiles;

		roomEmpty = RoomMask.IsEmpty(X, Y);

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

unsigned int AMuseum::ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction)
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

void AMuseum::GetFittingRoom(const int Width, const int Depth, const EDirection PlacementDirection, UClass*& OutRoom, FRotator& OutRoomRotation) const
{
	if (Width == 0 || Depth == 0)
	{
		return;
	}

	OutRoom = nullptr;
	OutRoomRotation = FRotator(0.0f, 0.0f, 0.0f);


	UClass* possibleRoom = nullptr;
	bool possibleRoomShouldBeRotated = false;
	bool hasFoundRoom = false;

	TArray<FIntPoint> roomDoors;

	for (size_t i = 0; i < PossibleRooms.Num() && !hasFoundRoom; ++i)
	{
		auto size = Cast<ARoomTemplate>(PossibleRooms[i]->GetDefaultObject())->RoomSize;
		if (size.X <= Width && size.Y <= Depth)
		{
			possibleRoom = PossibleRooms[i];
			possibleRoomShouldBeRotated = false;
		}
		else if (size.X <= Depth && size.Y <= Width)
		{
			possibleRoom = PossibleRooms[i];
			possibleRoomShouldBeRotated = true;
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
				|| (possibleRoomShouldBeRotated && (PlacementDirection == EDirection::Up || PlacementDirection == EDirection::Down)))
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
			else if ((PlacementDirection == EDirection::Right && !possibleRoomShouldBeRotated) 
					 || (possibleRoomShouldBeRotated && (PlacementDirection == EDirection::Up || PlacementDirection == EDirection::Down)))
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
			else if ((PlacementDirection == EDirection::Up && !possibleRoomShouldBeRotated)
					 || (possibleRoomShouldBeRotated && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right)))
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
			else if ((PlacementDirection == EDirection::Down && !possibleRoomShouldBeRotated)
					 || (possibleRoomShouldBeRotated && (PlacementDirection == EDirection::Left || PlacementDirection == EDirection::Right)))
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
