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
	~AVents();

	void SetVentGrid(class FMapGrid* Grid);

protected:
	virtual void BeginPlay() override;

private:
	FMapGrid* CreatePlaceholderVents();
	void PlaceVents(const FMapGrid* Grid);


public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* VentMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* VentMaterial = nullptr;

private:
	class FMapGrid* VentGrid;
};
