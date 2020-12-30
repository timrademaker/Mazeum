// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vents.generated.h"

UCLASS()
class UNREALSFAS_API AVents : public AActor
{
	GENERATED_BODY()
	
public:
	AVents();

	/** Place vents based on a grid */
	void PlaceVents(const class FMapGrid& Grid);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* VentMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* VentMaterial = nullptr;
};
