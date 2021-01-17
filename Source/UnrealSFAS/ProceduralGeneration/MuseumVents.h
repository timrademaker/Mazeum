// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumVents.generated.h"

UCLASS()
class UNREALSFAS_API AMuseumVents : public AActor
{
	GENERATED_BODY()
	
public:
	AMuseumVents();

	/**
	 * Place vents based on a grid
	 * @param Grid The grid representing the vent layout
	 */
	void PlaceVents(const class FMapGrid& Grid);

public:
	/** The mesh to use for the vents */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh to use for the vents"))
	UStaticMesh* VentMesh = nullptr;

	/** The material to use for the vents */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The material to use for the vents"))
	UMaterialInterface* VentMaterial = nullptr;
};
