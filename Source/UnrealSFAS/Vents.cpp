// Fill out your copyright notice in the Description page of Project Settings.


#include "Vents.h"

#include "ConstantsFunctionLibrary.h"
#include "MapGrid.h"


AVents::AVents()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AVents::BeginPlay()
{
	Super::BeginPlay();
	
	// Placeholder for the hand-made map
	FMapGrid grid(10, 11);
	grid.SetRow(2, 0b0000000010);
	grid.SetRow(3, 0b0000001110);
	grid.SetRow(4, 0b0000011010);
	grid.SetRow(5, 0b0000010010);
	grid.SetRow(6, 0b0000010010);
	grid.SetRow(7, 0b0000111110);
	grid.SetRow(8, 0b0000100010);
	grid.SetRow(9, 0b0000111110);
	grid.SetRow(10, 0b0000000100);

	PlaceVents(grid);
}

void AVents::PlaceVents(const FMapGrid& Grid)
{
	if (VentMesh)
	{
		const FVector ventBlockSize = UConstantsFunctionLibrary::GetVentBlockSize();
		const float blockZPos = GetActorLocation().Z - (0.5f * ventBlockSize.Z) - 1.0f;

		FVector2D blockPos(0.0f);
		FIntPoint mazeSize(Grid.GetWidth(), Grid.GetDepth());

		const FQuat worldRotation(FVector(0.0f, 0.0f, 1.0f), 0.0f);
		const FVector worldScale = UConstantsFunctionLibrary::GetVentBlockScale();

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the vents as static mesh components attached to the root of this actor
		// Start at -1 and continue to maze size to also place a block on the outer edge
		for (int32 i = -1; i <= mazeSize.Y; i++)
		{
			blockPos.Y = (static_cast<float>(i) + 0.5f) * ventBlockSize.Y;

			for (int32 j = -1; j <= mazeSize.X; j++)
			{
				blockPos.X = (static_cast<float>(j) + 0.5f)* ventBlockSize.X;

				FVector worldPosition(blockPos, blockZPos);

				if (!Grid.IsEmpty(j, i))
				{
					// Place a block under the vent (to stand on)
					worldPosition.Z -= ventBlockSize.Z;
				}

				UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this);
				FTransform worldXForm(worldRotation, worldPosition, worldScale);

				meshComponent->SetStaticMesh(VentMesh);

				if (VentMaterial)
				{
					meshComponent->SetMaterial(0, VentMaterial);
				}

				meshComponent->SetWorldTransform(worldXForm);
				meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				meshComponent->RegisterComponent();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Unable to create vents as the vent mesh is not set!"));
	}
}

