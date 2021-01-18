// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingBlockActorBase.h"
#include "PropSizeEnum.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomProp.generated.h"

/** Actor that is turned into a random prop when the game starts */
UCLASS()
class MAZEUM_API ARandomProp : public ABuildingBlockActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomProp();

	/// ABuildingBlockActorBase interface
	virtual void SetUpBuildingBlock(const UBuildingBlockMeshComponent* BuildingBlockComponent) override;
	/// End of ABuildingBlockActorBase interface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** Choose the prop to use for this random prop and set it up */
	void SetUpProp();

public:
	/** The size of the prop to place at this actor's location */
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The size of the prop to place at this actor's location"))
	EPropSize PropSize;

	/** Props that fit on the small pedestal */
	UPROPERTY(EditDefaultsOnly, Category = Meshes, meta = (ToolTip = "Props that fit on the small pedestal"))
	TArray<UStaticMesh*> SmallProps;

	UPROPERTY(EditDefaultsOnly, Category = Meshes)
	TArray<UStaticMesh*> MediumProps;

	/** Props that fit on the wide pedestal */
	UPROPERTY(EditDefaultsOnly, Category = Meshes, meta = (ToolTip = "Props that fit on the wide pedestal"))
	TArray<UStaticMesh*> LargeProps;

protected:
	/** The static mesh for this prop */
	UStaticMeshComponent* StaticMesh;

	/** The collider to use for this prop */
	class UBoxComponent* BoxCollider;
};
