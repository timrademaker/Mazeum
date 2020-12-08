// Fill out your copyright notice in the Description page of Project Settings.


#include "Museum.h"

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
	FMapGrid roomMask;
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
				// TODO: Potential issue: This could leave out corner tiles in some directions if there's empty space above the tile, but not enough to place a room


				// Determine the number of walls with adjacent empty space
				// Variables representing the dimensions of the empty space
				int emptyDepth = 0;
				int emptyWidth = 0;

				if (dir == EDirection::Left || dir == EDirection::Right)
				{
					emptyDepth = emptyTiles;
					emptyWidth = ContiguousUnoccupiedWallCount(MuseumLayout, RoomMask, x, y, EDirection::Down);

				}
				else
				{
					emptyDepth = ContiguousUnoccupiedWallCount(MuseumLayout, RoomMask, x, y, EDirection::Right);
					emptyWidth = emptyTiles;
				}

				// Find a suitable room for these dimensions
				UClass* room = nullptr;
				bool roomShouldBeRotated = false;
				GetFittingRoom(emptyWidth, emptyDepth, room, roomShouldBeRotated);

				if (room)
				{
					// TODO: Determine how the room should be rotated (based on roomShouldBeRotated and dir)
					FRoomPlacement placement;
					placement.Position = FIntVector(x, y, 0);
					placement.RoomType = room;
					placement.Rotation = FRotator();
					OutRoomPlacement.Add(placement);

					// TODO: Fill RoomMask (based on roomShouldBeRotated and dir)
					//  Remember: Current x, y is a wall
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

void AMuseum::PlaceRooms(const TArray<FRoomPlacement>& Rooms)
{
	// TODO: Implement
	check(false);
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

unsigned int AMuseum::ContiguousUnoccupiedWallCount(const FMapGrid& MuseumLayout, const FMapGrid& RoomMask, int X, int Y, const EDirection Direction)
{
	int unoccupiedWallCount = 0;

	int maxX = MuseumLayout.GetWidth() - 1;
	int maxY = MuseumLayout.GetDepth() - 1;

	while (X > 0 && X < maxX
		&& Y > 0 && Y < maxY
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

void AMuseum::GetFittingRoom(const int Width, const int Depth, UClass* OutRoom, bool& OutShouldBeRotated) const
{
	for (size_t i = 0; i < PossibleRooms.Num(); ++i)
	{
		auto size = Cast<ARoomTemplate>(PossibleRooms[i])->RoomSize;
		if (size.X <= Width && size.Y <= Depth)
		{
			OutRoom = PossibleRooms[i];
			OutShouldBeRotated = false;
			break;
		}
		else if (size.X <= Depth && size.Y <= Width)
		{
			OutRoom = PossibleRooms[i];
			OutShouldBeRotated = true;
			break;
		}
	}
}
