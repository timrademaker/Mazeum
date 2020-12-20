// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoomBuildingBlocks/BuildingBlockMeshComponent.h"

#include "RoomTemplate.generated.h"

class UBoxComponent;
class UDataTable;

UCLASS()
class UNREALSFAS_API ARoomTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomTemplate();

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

private:
	void UpdateBlockPlacementTable();


public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The size of the room template in tiles"))
	FIntVector RoomSize = FIntVector(5, 5, 5);

	UPROPERTY(EditDefaultsOnly)
	UDataTable* BuildingBlockPlacementTable;
	
protected:
	UBoxComponent* RoomBounds;
};
