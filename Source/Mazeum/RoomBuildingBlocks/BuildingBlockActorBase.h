// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBlockActorBase.generated.h"

UCLASS()
class MAZEUM_API ABuildingBlockActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingBlockActorBase();

	/**
	 * Sets up the building block actor based on its component equivalent
	 * @param BuildingBlockComponent The building block component to use in order to set up the actor
	 */
	virtual void SetUpBuildingBlock(const class UBuildingBlockMeshComponent* BuildingBlockComponent) {};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
