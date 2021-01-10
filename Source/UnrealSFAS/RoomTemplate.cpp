// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplate.h"

#include "BuildingBlockPlacementStruct.h"
#include "ConstantsFunctionLibrary.h"
#include "StealthGameMode.h"
#include "Gameplay/Lasers.h"
#include "ProceduralGeneration/RoomPlacement.h"
#include "RoomBuildingBlocks/BuildingBlockMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

#if WITH_EDITOR
#include "EditorAssetLibrary.h"
#endif


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

#if WITH_EDITOR
	UPackage::PackageSavedEvent.AddUObject(this, &ARoomTemplate::OnPackageSaved);
	ClassName = GetClass()->GetName().Replace(TEXT("_C"), TEXT(""));
#endif
}

ARoomTemplate::~ARoomTemplate()
{
#if WITH_EDITOR
	UPackage::PackageSavedEvent.RemoveAll(this);
#endif
}

void ARoomTemplate::OnConstruction(const FTransform& Transform)
{
	FVector blockDimensions = UConstantsFunctionLibrary::GetBlockSize();
	
	// Resize room bounds
	const FVector newExtent = FVector(RoomSize) * blockDimensions * 0.5f;
	RoomBounds->SetBoxExtent(newExtent, false);
	RoomBounds->SetRelativeLocation(newExtent);

#if WITH_EDITOR
	BuildingBlockPlacementTableIsDirty = true;
#endif
}

void ARoomTemplate::GetLocationsOfBlocksWithType(const EBuildingBlockType BlockType, const TSubclassOf<ARoomTemplate> RoomType, TArray<FIntPoint>& BlockLocations)
{
	ARoomTemplate* defaultRoom = Cast<ARoomTemplate>(RoomType->GetDefaultObject());
	if (defaultRoom->BuildingBlockPlacementTable)
	{
		// Search data table for this type of room and building block
		TArray<FBuildingBlockPlacementStruct*> rows;
		defaultRoom->BuildingBlockPlacementTable->GetAllRows("RoomTemplate GetLocationsOfBlocksWithType", rows);

		for (FBuildingBlockPlacementStruct* row : rows)
		{
			if (row->RoomType == RoomType && row->BlockType == BlockType)
			{
				BlockLocations = row->BlockLocation;
				return;
			}
		}
	}
}

void ARoomTemplate::ConvertToRoom(const FRoomPlacement& RoomPlacement)
{
	RoomBounds->SetRelativeRotation(RoomPlacement.Rotation);

	// Convert room components to actors
	// For each component
	// Replace with actor equivalent
	TArray<USceneComponent*> components;
	RoomBounds->GetChildrenComponents(true, components);

	for (USceneComponent* comp : components)
	{
		UBuildingBlockMeshComponent* componentAsBuildingBlock = Cast<UBuildingBlockMeshComponent>(comp);
		if (!componentAsBuildingBlock)
		{
			continue;
		}

		TSubclassOf<ABuildingBlockActorBase> actorClass = componentAsBuildingBlock->ActorEquivalent;
		if (actorClass)
		{
			ABuildingBlockActorBase* obj = GetWorld()->SpawnActorDeferred<ABuildingBlockActorBase>(actorClass, FTransform());
			obj->SetUpBuildingBlock(componentAsBuildingBlock);
			obj->FinishSpawning(FTransform());

			obj->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			obj->AddActorLocalTransform(RoomBounds->GetRelativeTransform());
			obj->AddActorLocalTransform(comp->GetRelativeTransform());

			if (componentAsBuildingBlock->BuildingBlockType == EBuildingBlockType::Door)
			{
				Cast<ALasers>(obj)->SetLasersEnabled(RoomPlacement.LasersAreEnabled);
			}

			// Remove component from room template actor
			comp->UnregisterComponent();
		}
	}
}

// Called when the game starts or when spawned
void ARoomTemplate::BeginPlay()
{
	Super::BeginPlay();

	AStealthGameMode* gm = GetWorld()->GetAuthGameMode<AStealthGameMode>();
	if (gm)
	{
		GuardPaths.Empty();
		GetComponents<UGuardPatrolPathComponent>(GuardPaths);

		gm->RegisterGuardPatrolPathArray(&GuardPaths);
	}
}

#if WITH_EDITOR
void ARoomTemplate::UpdateBlockPlacementTable()
{
	if (GetWorld() && GetWorld()->IsPlayInEditor())
	{
		return;
	}

	if (!BuildingBlockPlacementTable)
	{
		return;
	}

	FVector blockDimensions = UConstantsFunctionLibrary::GetBlockSize();

	// Remove existing rows for this room
	TArray<FBuildingBlockPlacementStruct*> rows;
	BuildingBlockPlacementTable->GetAllRows("RoomTemplate construction script", rows);

	TArray<FName> rowNames = BuildingBlockPlacementTable->GetRowNames();

	for (int i = 0; i < rows.Num(); ++i)
	{
		if (rows[i]->RoomType == GetClass())
		{
			BuildingBlockPlacementTable->RemoveRow(rowNames[i]);
		}
	}

	// Go through all components, find (special) building blocks and store their location in a data table
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
				FIntPoint tilePosition = FIntPoint(FMath::FloorToInt(tilePositionInRoom.X), FMath::FloorToInt(tilePositionInRoom.Y));

				// See if a row for this block type already exists
				FString rowName = ClassName + FString::FromInt(static_cast<uint8>(block->BuildingBlockType));

				FBuildingBlockPlacementStruct* foundRow = BuildingBlockPlacementTable->FindRow<FBuildingBlockPlacementStruct>(FName(rowName), "RoomTemplate construction script");
				if (foundRow)
				{
					foundRow->BlockLocation.Add(tilePosition);
				}
				else
				{
					FBuildingBlockPlacementStruct row;
					row.RoomType = GetClass();
					row.BlockType = block->BuildingBlockType;
					row.BlockLocation.Empty();
					row.BlockLocation.Add(tilePosition);

					BuildingBlockPlacementTable->AddRow(FName(rowName), row);
				}
			}
		}
	}
}

void ARoomTemplate::OnPackageSaved(const FString& PackageFileName, UObject* PackageObj)
{	
	if (BuildingBlockPlacementTable && BuildingBlockPlacementTableIsDirty)
	{
		FString packageName = PackageObj->GetName();

		if (packageName.EndsWith(ClassName))
		{
			check(BuildingBlockPlacementTable->GetRowStruct() == FBuildingBlockPlacementStruct::StaticStruct() && "BuildingBlockPlacementTable should use BuildingBlockPlacementStruct as row struct!");

			UpdateBlockPlacementTable();
			UEditorAssetLibrary::SaveAsset(BuildingBlockPlacementTable->GetPathName(), false);

			BuildingBlockPlacementTableIsDirty = false;
		}
	}
}
#endif