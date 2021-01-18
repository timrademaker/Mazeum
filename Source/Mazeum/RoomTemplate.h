// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Gameplay/GuardPatrolPath.h"
#include "RoomBuildingBlocks/BuildingBlockMeshComponent.h"
#include "Gameplay/PickUpBase.h"

#include "RoomTemplate.generated.h"


UCLASS()
class MAZEUM_API ARoomTemplate : public AActor
{
	GENERATED_BODY()
	
public:
	ARoomTemplate();
	~ARoomTemplate();

	/** Construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** 
	 * Gets the location of all building block of a certain type in this type of room 
	 * @param BlockType The type of block to look for
	 * @param RoomType The room class that should be searched
	 * @param BlockLocations The found locations of the building blocks of the specified type
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetLocationsOfBlocksWithType(const EBuildingBlockType BlockType, const TSubclassOf<ARoomTemplate> RoomType, TArray<FIntPoint>& BlockLocations);

	/**
	 * Convert this room's components to actors, if possible
	 * @param RoomPlacement The room placement info
	 */
	void ConvertToRoom(const struct FRoomPlacement& RoomPlacement);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
private:
	/** Update the table with building block positions */
	void UpdateBlockPlacementTable();
	/** Callback for UPackage::PackageSavedEvent */
	void OnPackageSaved(const FString& PackageFileName, UObject* PackageObj);
#endif

public:
	/** The size of the room template in tiles */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The size of the room template in tiles"))
	FIntVector RoomSize = FIntVector(5, 5, 5);

	/** The paths guards can follow in this room */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The paths guards can follow in this room"))
	TArray<UGuardPatrolPathComponent*> GuardPaths;

	/** The table to store building block placement positions in. Should use BuildingBlockPlacementStruct as row structure */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The table to store building block placement positions in. Should use BuildingBlockPlacementStruct as row structure"))
	class UDataTable* BuildingBlockPlacementTable;

	/** The class to use for pick-ups */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The class to use for pick-ups"))
	TSubclassOf<APickUpBase> PickupClass;
	
protected:
	/** The bounds of the room */
	class UBoxComponent* RoomBounds;

#if WITH_EDITOR
private:
	/** The name of the class, without _C suffix */
	FString ClassName;
	/** Set to true if the building block placement table has been modified but has not been saved since */
	bool BuildingBlockPlacementTableIsDirty = false;
#endif
};
