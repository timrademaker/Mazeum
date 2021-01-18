// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseumVents.h"

#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"


AMuseumVents::AMuseumVents()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMuseumVents::PlaceVents(const FMapGrid& Grid)
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
				meshComponent->SetReceivesDecals(false);

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

