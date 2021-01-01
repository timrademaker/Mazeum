// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseumFloor.h"

#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"

// Sets default values
AMuseumFloor::AMuseumFloor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMuseumFloor::PlaceFloor(const FMapGrid& Grid)
{
	if (FloorMeshComponent)
	{
		const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
		const float blockZPos = GetActorLocation().Z;

		FVector2D blockPos(0.0f);
		const FIntPoint gridSize(Grid.GetWidth(), Grid.GetDepth());

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the floor tiles as static mesh components attached to the root of this actor
		for (int32 i = 0; i < gridSize.Y; i++)
		{
			for (int32 j = 0; j < gridSize.X; j++)
			{
				if (Grid.IsEmpty(j, i))
				{
					continue;
				}

				blockPos.X = (static_cast<float>(j) + FloorComponentAlignment.X) * blockSize.X;
				blockPos.Y = (static_cast<float>(i) + FloorComponentAlignment.Y) * blockSize.Y;

				const FVector worldPosition(blockPos, blockZPos);

				UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this, FloorMeshComponent);

				meshComponent->SetWorldLocation(worldPosition);
				meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				meshComponent->RegisterComponent();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Unable to create floor as the floor mesh component is not set!"));
	}
}

// Called when the game starts or when spawned
void AMuseumFloor::BeginPlay()
{
	Super::BeginPlay();
	

	// Placeholder for the hand-made map
	FMapGrid grid(10, 11);

	grid.SetRow(3 , 0b1100000000);
	grid.SetRow(4 , 0b1111111111);
	grid.SetRow(5 , 0b1111111111);
	grid.SetRow(6 , 0b1111111111);
	grid.SetRow(7 , 0b1100000000);

	PlaceFloor(grid);

}