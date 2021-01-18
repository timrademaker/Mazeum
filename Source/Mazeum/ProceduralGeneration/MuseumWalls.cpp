// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseumWalls.h"

#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"

#include "Components/BoxComponent.h"

struct FWallPlacement
{
	FVector Position;
	FRotator Rotation;
};

// Sets default values
AMuseumWalls::AMuseumWalls()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMuseumWalls::PlaceWalls(const FMapGrid& HallGrid, const class FMapGrid& DoorLocations)
{
	if (WallMeshComponent)
	{
		const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
		const float blockZPos = GetActorLocation().Z;
		const FIntPoint gridSize(HallGrid.GetWidth(), HallGrid.GetDepth());
		const FVector wallExtent = Cast<UStaticMeshComponent>(WallMeshComponent->GetDefaultObject())->GetStaticMesh()->GetBounds().BoxExtent;

		USceneComponent* rootComponent = GetRootComponent();

		TArray<FWallPlacement> walls;
		walls.Reserve(4);

		// Loop through the binary values to generate the wall tiles as static mesh components attached to the root of this actor
		for (int32 y = 0; y < gridSize.Y; y++)
		{
			for (int32 x = 0; x < gridSize.X; x++)
			{
				if (HallGrid.IsEmpty(x, y))
				{
					continue;
				}

				// Check left side of the tile
				if (x == 0 || (HallGrid.IsEmpty(x - 1, y) && DoorLocations.IsEmpty(x - 1, y)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, -90.0f, 0.0f);
					wall.Position.X = (static_cast<float>(x)) * blockSize.X;
					wall.Position.Y = (static_cast<float>(y) + 1.0f - WallComponentAlignment.Y) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check right side of the tile
				if (x == gridSize.X - 1 || (HallGrid.IsEmpty(x + 1, y) && DoorLocations.IsEmpty(x + 1, y)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 90.0f, 0.0f);
					wall.Position.X = (static_cast<float>(x) + 1.0f) * blockSize.X;
					wall.Position.Y = (static_cast<float>(y) + WallComponentAlignment.Y) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check top side of the tile
				if (y == 0 || (HallGrid.IsEmpty(x, y - 1) && DoorLocations.IsEmpty(x , y - 1)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 0.0f, 0.0f);
					wall.Position.X = (static_cast<float>(x) + WallComponentAlignment.X) * blockSize.X;
					wall.Position.Y = (static_cast<float>(y)) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check bottom side of the tile
				if (y == gridSize.Y - 1 || (HallGrid.IsEmpty(x, y + 1) && DoorLocations.IsEmpty(x, y + 1)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 180.0f, 0.0f);
					wall.Position.X = (static_cast<float>(x) + 1.0f - WallComponentAlignment.X) * blockSize.X;
					wall.Position.Y = (static_cast<float>(y) + 1.0f) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				for (auto& wall : walls)
				{
					UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this, WallMeshComponent);

					meshComponent->SetWorldLocation(wall.Position);
					meshComponent->SetWorldRotation(wall.Rotation);
					meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
					meshComponent->RegisterComponent();

					// Also add a collider to the wall
					UBoxComponent* collider = NewObject<UBoxComponent>(meshComponent);
					collider->SetBoxExtent(wallExtent);
					collider->AttachToComponent(meshComponent, FAttachmentTransformRules::KeepRelativeTransform);
					collider->AddLocalOffset(wallExtent);
					collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
					collider->RegisterComponent();
				}

				walls.Empty(4);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Unable to create walls as the wall mesh component is not set!"));
	}
}
