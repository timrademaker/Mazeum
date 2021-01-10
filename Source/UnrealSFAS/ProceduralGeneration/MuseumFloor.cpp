// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseumFloor.h"

#include "../ConstantsFunctionLibrary.h"
#include "../MapGrid.h"

// Sets default values
AMuseumFloor::AMuseumFloor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMuseumFloor::PlaceFloor(const FMapGrid& HallMask, const FMapGrid& RoomMask, const FMapGrid& VentEntranceMask, const FMapGrid& VentMask)
{
	if (FloorTileMeshComponent)
	{
		const FVector blockSize = UConstantsFunctionLibrary::GetBlockSize();
		const float blockZPos = GetActorLocation().Z;

		FVector2D blockPos(0.0f);
		const FIntPoint gridSize(HallMask.GetWidth(), HallMask.GetDepth());

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the floor tiles as static mesh components attached to the root of this actor
		for (int32 y = 0; y < gridSize.Y; y++)
		{
			for (int32 x = 0; x < gridSize.X; x++)
			{
				if ((HallMask.IsEmpty(x, y) && RoomMask.IsEmpty(x, y) && VentMask.IsEmpty(x, y)) || !VentEntranceMask.IsEmpty(x, y))
				{
					continue;
				}

				blockPos.X = (static_cast<float>(x) + FloorComponentAlignment.X) * blockSize.X;
				blockPos.Y = (static_cast<float>(y) + FloorComponentAlignment.Y) * blockSize.Y;

				const FVector worldPosition(blockPos, blockZPos);

				UStaticMeshComponent* floorMeshComponent = NewObject<UStaticMeshComponent>(this, FloorTileMeshComponent);

				floorMeshComponent->SetWorldLocation(worldPosition);
				floorMeshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				floorMeshComponent->RegisterComponent();

				if (VentTileMeshComponent)
				{
					UStaticMeshComponent* ventMeshComponent = NewObject<UStaticMeshComponent>(floorMeshComponent, VentTileMeshComponent);
					ventMeshComponent->AttachToComponent(floorMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
					ventMeshComponent->AddLocalOffset(FVector(0.0f, 0.0f, -200.0f));
					ventMeshComponent->SetRelativeScale3D(FVector(1.0f));
					ventMeshComponent->AddLocalRotation(FRotator(0.0f, 0.0f, 180.0f));
					ventMeshComponent->RegisterComponent();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("Unable to create floor as the floor mesh component is not set!"));
	}
}
