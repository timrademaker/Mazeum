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
		for (int32 i = 0; i < gridSize.Y; i++)
		{
			for (int32 j = 0; j < gridSize.X; j++)
			{
				if (HallGrid.IsEmpty(j, i))
				{
					continue;
				}

				// Check left side of the tile
				if (j == 0 || (HallGrid.IsEmpty(j - 1, i) && DoorLocations.IsEmpty(j - 1, i)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, -90.0f, 0.0f);
					wall.Position.X = (static_cast<float>(j)) * blockSize.X;
					wall.Position.Y = (static_cast<float>(i) + 1.0f - WallComponentAlignment.Y) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check right side of the tile
				if (j == gridSize.X - 1 || (HallGrid.IsEmpty(j + 1, i) && DoorLocations.IsEmpty(j + 1, i)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 90.0f, 0.0f);
					wall.Position.X = (static_cast<float>(j) + 1.0f) * blockSize.X;
					wall.Position.Y = (static_cast<float>(i) + WallComponentAlignment.Y) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check top side of the tile
				if (i == 0 || (HallGrid.IsEmpty(j, i - 1) && DoorLocations.IsEmpty(j , i - 1)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 0.0f, 0.0f);
					wall.Position.X = (static_cast<float>(j) + WallComponentAlignment.X) * blockSize.X;
					wall.Position.Y = (static_cast<float>(i)) * blockSize.Y;
					wall.Position.Z = blockZPos;

					walls.Add(wall);
				}

				// Check bottom side of the tile
				if (j == gridSize.Y - 1 || (HallGrid.IsEmpty(j, i + 1) && DoorLocations.IsEmpty(j, i + 1)))
				{
					FWallPlacement wall;
					wall.Rotation = FRotator(0.0f, 180.0f, 0.0f);
					wall.Position.X = (static_cast<float>(j) + 1.0f - WallComponentAlignment.X) * blockSize.X;
					wall.Position.Y = (static_cast<float>(i) + 1.0f) * blockSize.Y;
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

// Called when the game starts or when spawned
void AMuseumWalls::BeginPlay()
{
	Super::BeginPlay();
	
	// Placeholder for the hand-made map
	FMapGrid hallGrid(10, 11);

	hallGrid.SetRow(3, 0b1100000000);
	hallGrid.SetRow(4, 0b1111111111);
	hallGrid.SetRow(5, 0b1111111111);
	hallGrid.SetRow(6, 0b1111111111);
	hallGrid.SetRow(7, 0b1100000000);

	FMapGrid doorLocations(10, 11);
	doorLocations.SetRow(3, 0b0001001001);
	doorLocations.SetRow(7, 0b0001001010);

	PlaceWalls(hallGrid, doorLocations);
}


