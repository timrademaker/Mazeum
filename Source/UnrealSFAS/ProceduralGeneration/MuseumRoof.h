// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../MapGrid.h"

#include "MuseumRoof.generated.h"

UCLASS()
class UNREALSFAS_API AMuseumRoof : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMuseumRoof();

	/** Place roof tiles based on a grid */
	void PlaceRoof(const class FMapGrid& Grid);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> RoofMeshComponent = nullptr;
};
