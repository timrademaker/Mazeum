// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Museum.generated.h"

UCLASS()
class UNREALSFAS_API AMuseum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuseum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/* Placement */
	void PlaceRooms(const TArray<struct FRoomPlacement>& Rooms);

public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The rooms to use when generating the museum"))
	TArray<TSubclassOf<class ARoomTemplate>> PossibleRooms;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The class used to place walls"))
	TSubclassOf<class AMuseumWalls> MuseumWallsClass;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The class used to place ceilings"))
	TSubclassOf<class AMuseumCeiling> MuseumCeilingClass;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The class used to create the museum's floor"))
	TSubclassOf<class AMuseumFloor> MuseumFloorClass;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The class used to place vents"))
	TSubclassOf<class AMuseumVents> MuseumVentsClass;
};
