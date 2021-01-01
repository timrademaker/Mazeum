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

	/** Place floor tiles based on a grid */
	void PlaceFloor(const class FMapGrid& Grid);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The alignment of the mesh in the floor component. (0, 0) means that the mesh starts at local (0,0) coordinate and extends in the positive X and Y direction"))
	FVector2D FloorComponentAlignment = FVector2D(0.5f);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStaticMeshComponent> FloorMeshComponent = nullptr;
};
