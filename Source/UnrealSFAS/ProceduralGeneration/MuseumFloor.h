// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumFloor.generated.h"

UCLASS()
class UNREALSFAS_API AMuseumFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuseumFloor();

	/** 
	 * Place floor tiles based on a grid
	 * @param HallMask The museum's hall mask
	 * @param RoomMask The museum's room mask
	 * @param VentEntranceMask The museum's vent entrance mask
	 * @param VentMask The vent layout mask
	 */
	void PlaceFloor(const class FMapGrid& HallMask, const class FMapGrid& RoomMask, const class FMapGrid& VentEntranceMask, const class FMapGrid& VentMask);

public:	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The alignment of the mesh in the floor component. (0, 0) means that the mesh starts at local (0,0) coordinate and extends in the positive X and Y direction"))
	FVector2D FloorComponentAlignment = FVector2D(0.5f);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> FloorTileMeshComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> VentTileMeshComponent = nullptr;
};
