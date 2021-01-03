// Fill out your copyright notice in the Description page of Project Settings.


#include "Museum.h"

#include "MuseumGenerator.h"
#include "RoomPlacement.h"
#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"

#include "MuseumCeiling.h"
#include "MuseumFloor.h"
#include "MuseumVents.h"
#include "MuseumWalls.h"


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

	FMapGrid hallMask;
	FMapGrid ventEntranceMask;
	FMapGrid ventMask;
	FMapGrid doorMask;
	TArray<FRoomPlacement> roomPlacement;

	FMuseumGenerator::GenerateMuseum(PossibleRooms, roomPlacement, hallMask, ventEntranceMask, doorMask, ventMask);


	// Create floor actor
	AMuseumFloor* floor = GetWorld()->SpawnActor<AMuseumFloor>(MuseumFloorClass);
	floor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	floor->SetActorRelativeLocation(FVector(0.0f));
	floor->PlaceFloor(hallMask);

	// Create ceiling actor
	AMuseumCeiling* ceiling = GetWorld()->SpawnActor<AMuseumCeiling>(MuseumCeilingClass);
	ceiling->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	ceiling->SetActorRelativeLocation(FVector(0.0f));
	ceiling->PlaceCeiling(hallMask);

	// Create walls actor
	AMuseumWalls* walls = GetWorld()->SpawnActor<AMuseumWalls>(MuseumWallsClass);
	walls->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	walls->SetActorRelativeLocation(FVector(0.0f));
	walls->PlaceWalls(hallMask, doorMask);

	// Create vent actor
	AMuseumVents* vents = GetWorld()->SpawnActor<AMuseumVents>(MuseumVentsClass);
	vents->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	vents->SetActorRelativeLocation(FVector(0.0f));
	vents->PlaceVents(ventMask);

	PlaceRooms(roomPlacement);
}


void AMuseum::PlaceRooms(const TArray<FRoomPlacement>& Rooms)
{
	const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
	const FVector museumLocation = GetActorLocation();

	for (const auto& room : Rooms)
	{
		FVector roomLocation = museumLocation + FVector(room.Position.X, room.Position.Y, 0.0f) * blockSize;
		const FVector realRoomSize = FVector(room.RoomType->GetDefaultObject<ARoomTemplate>()->RoomSize) * blockSize;

		// Adjust spawning location, as this happens right-down, while the tile a room is spawned on is the bottom left or top right for Up and Left
		if (room.Direction == EDirection::Up)
		{
			roomLocation.Y -= (realRoomSize.Y - blockSize.Y);
		}
		else if (room.Direction == EDirection::Left)
		{
			roomLocation.X -= (realRoomSize.X - blockSize.X);
		}

		if (room.Rotation.Equals(FRotator(0.0f, -90.0f, 0.0f)) || room.Rotation.Equals(FRotator(0.0f, 90.0f, 0.0f)))
		{
			// Move the room left and up by half of the difference in the room's dimensions to shift the room's origin to the correct location
			const FIntVector roomSize = room.RoomType->GetDefaultObject<ARoomTemplate>()->RoomSize;
			const int diff = FMath::Abs(roomSize.Y - roomSize.X);
			
			if (room.Direction == EDirection::Left)
			{
				roomLocation.X += (diff / 2) * blockSize.X;
				roomLocation.Y += (diff / 2) * blockSize.Y;
			}
			else if (room.Direction == EDirection::Down || room.Direction == EDirection::Right)
			{
				roomLocation.X -= (diff / 2) * blockSize.X;
				roomLocation.Y += (diff / 2) * blockSize.Y;
			}
			else if(room.Direction == EDirection::Up)
			{
				roomLocation.X -= (diff / 2) * blockSize.X;
				roomLocation.Y -= (diff / 2) * blockSize.Y;
			}
		}

		// Spawn actor and convert to an actual room
		ARoomTemplate* r = GetWorld()->SpawnActor<ARoomTemplate>(room.RoomType, roomLocation, GetActorRotation());
		r->ConvertToRoom(room.Rotation);
		r->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	}
}

