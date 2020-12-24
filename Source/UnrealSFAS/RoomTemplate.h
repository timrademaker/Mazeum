// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Gameplay/GuardPatrolPath.h"
#include "RoomBuildingBlocks/BuildingBlockMeshComponent.h"

#include "RoomTemplate.generated.h"


class UBoxComponent;
class UDataTable;

UCLASS()
class UNREALSFAS_API ARoomTemplate : public AActor
{
	GENERATED_BODY()
	
public:
	ARoomTemplate();
	~ARoomTemplate();

	virtual void OnConstruction(const FTransform& Transform) override;

	/*
	* @brief Gets the location of all building block of a certain type in this type of room
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetLocationsOfBlocksWithType(const EBuildingBlockType BlockType, const TSubclassOf<ARoomTemplate> RoomType, TArray<FIntPoint>& BlockLocations);

	void SetRoomRotation(const FRotator Rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
private:
	void UpdateBlockPlacementTable();
	void OnPackageSaved(const FString& PackageFileName, UObject* PackageObj);
#endif

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The size of the room template in tiles"))
	FIntVector RoomSize = FIntVector(5, 5, 5);

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The paths guards can follow in this room"))
	TArray<UGuardPatrolPathComponent*> GuardPaths;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* BuildingBlockPlacementTable;
	
protected:
	UBoxComponent* RoomBounds;

#if WITH_EDITOR
private:
	FString ClassName;
	bool BuildingBlockPlacementTableIsDirty = false;
#endif
};
