// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MuseumCeiling.generated.h"

UCLASS()
class MAZEUM_API AMuseumCeiling : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMuseumCeiling();

	/** 
	 * Place ceiling tiles based on a grid
	 * @param HallMask The museum's hall mask
	 * @param RoomMask The museum's room mask
	 */
	void PlaceCeiling(const class FMapGrid& HallMask, const class FMapGrid& RoomMask);

public:
	/** The mesh component to use as ceiling */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh component to use as ceiling"))
	TSubclassOf<UStaticMeshComponent> CeilingMeshComponent = nullptr;
};
