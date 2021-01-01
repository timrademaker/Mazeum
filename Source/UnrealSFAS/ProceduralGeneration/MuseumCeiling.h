// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MuseumCeiling.generated.h"

UCLASS()
class UNREALSFAS_API AMuseumCeiling : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMuseumCeiling();

	/** Place ceiling tiles based on a grid */
	void PlaceCeiling(const class FMapGrid& Grid);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> CeilingMeshComponent = nullptr;
};
