#include "MuseumCeiling.h"

#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"

AMuseumCeiling::AMuseumCeiling()
{
    PrimaryActorTick.bCanEverTick = false;
}


void AMuseumCeiling::PlaceCeiling(const FMapGrid& Grid)
{
	if (CeilingMeshComponent)
	{
		const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
		const float blockZPos = GetActorLocation().Z + blockSize.Z + 1.0f;

		FVector2D blockPos(0.0f);
		FIntPoint gridSize(Grid.GetWidth(), Grid.GetDepth());

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the ceiling tiles as static mesh components attached to the root of this actor
		for (int32 i = 0; i < gridSize.Y; i++)
		{
			blockPos.Y = (static_cast<float>(i) + 0.5f) * blockSize.Y;

			for (int32 j = 0; j < gridSize.X; j++)
			{
				if (Grid.IsEmpty(j, i))
				{
					continue;
				}

				blockPos.X = (static_cast<float>(j) + 0.5f) * blockSize.X;

				FVector worldPosition(blockPos, blockZPos);

				UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this, CeilingMeshComponent);

				meshComponent->SetWorldLocation(worldPosition);
				meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				meshComponent->RegisterComponent();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Unable to create ceiling as the ceiling mesh component is not set!"));
	}
}
