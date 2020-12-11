// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplate.h"

#include "Components/BoxComponent.h"

#include "ConstantsFunctionLibrary.h"

// Sets default values
ARoomTemplate::ARoomTemplate()
{
 	// No need to tick this actor
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* rootComponent = GetRootComponent();

	// Add a root component we can move the bounding box relative to
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent->SetupAttachment(rootComponent);
	
	// Create bounding box
	RoomBounds = CreateDefaultSubobject<UBoxComponent>("Room outline");
	RoomBounds->SetupAttachment(RootComponent);
}

void ARoomTemplate::OnConstruction(const FTransform& Transform)
{
	FVector blockDimensions = UConstantsFunctionLibrary::GetBlockSize();
	
	// Resize room bounds
	const FVector newExtent = FVector(RoomSize) * blockDimensions * 0.5f;
	RoomBounds->SetBoxExtent(newExtent, false);
	RoomBounds->SetRelativeLocation(newExtent);


	// Determine the tile location of some building blocks
	// Remove all previous block locations
	ARoomTemplate* defaultRoom = Cast<ARoomTemplate>(GetClass()->GetDefaultObject(true));
	
	defaultRoom->BuildingBlockLocations.Empty();

	TArray<USceneComponent*> components;
	RoomBounds->GetChildrenComponents(false, components);

	for (UActorComponent* component : components)
	{
		UBuildingBlockMeshComponent* block = Cast<UBuildingBlockMeshComponent>(component);
		if (block)
		{
			if (block->BuildingBlockType != EBuildingBlockType::Other)
			{
				// Calculate tile position based on coordinates in room
				FVector OffsetFromOrigin = FVector(RoomSize) * blockDimensions * 0.5f;

				FVector locationInRoom = block->GetRelativeLocation() + OffsetFromOrigin;
				FVector tilePositionInRoom = locationInRoom / blockDimensions;

				// Round down to get actual tile position
				defaultRoom->BuildingBlockLocations.FindOrAdd(block->BuildingBlockType).Add(FIntPoint(FMath::FloorToInt(tilePositionInRoom.X), FMath::FloorToInt(tilePositionInRoom.Y)));
			}
		}
	}
}

void ARoomTemplate::GetLocationsOfBlocksWithType(const EBuildingBlockType BlockType, const TSubclassOf<ARoomTemplate> RoomType, TArray<FIntPoint>& BlockLocations)
{
	ARoomTemplate* defaultRoom = Cast<ARoomTemplate>(RoomType->GetDefaultObject(true));
	if (defaultRoom)
	{
		BlockLocations = defaultRoom->BuildingBlockLocations.FindOrAdd(BlockType);
	}
	return;
}

// Called when the game starts or when spawned
void ARoomTemplate::BeginPlay()
{
	Super::BeginPlay();

}

