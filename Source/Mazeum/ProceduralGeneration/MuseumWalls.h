// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumWalls.generated.h"

UCLASS()
class MAZEUM_API AMuseumWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuseumWalls();

	/**
	 * Place walls based on a grid
	 * @param HallGrid The grid representing the hall layout
	 * @param DoorLocations The grid with door locations filled
	 */
	void PlaceWalls(const class FMapGrid& HallGrid, const class FMapGrid& DoorLocations);

public:
	/** The alignment of the mesh in the wall component. (0, 0) means that the mesh starts at local (0,0) coordinate and extends in the positive X and Y direction */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The alignment of the mesh in the wall component. (0, 0) means that the mesh starts at local (0,0) coordinate and extends in the positive X and Y direction"))
	FVector2D WallComponentAlignment = FVector2D(0.0f);

	/** The mesh component to use as wall */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh component to use as wall"))
	TSubclassOf<UStaticMeshComponent> WallMeshComponent = nullptr;

};
