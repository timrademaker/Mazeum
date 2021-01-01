// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumWalls.generated.h"

UCLASS()
class UNREALSFAS_API AMuseumWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuseumWalls();

	/** Place walls based on a grid */
	void PlaceWalls(const class FMapGrid& HallGrid, const class FMapGrid& DoorLocations);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The alignment of the mesh in the wall component. (0, 0) means that the mesh starts at local (0,0) coordinate and extends in the positive X and Y direction"))
	FVector2D WallComponentAlignment = FVector2D(0.0f);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> WallMeshComponent = nullptr;

};
