// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../RoomBuildingBlocks/BuildingBlockActorBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lasers.generated.h"

class UBoxComponent;
class UStaticMesh;

class UAlarmComponent;

UCLASS()
class MAZEUM_API ALasers : public ABuildingBlockActorBase
{
	GENERATED_BODY()
	
public:	
	ALasers();

	/** Construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Enable or disable the lasers
	 * @param Enabled True if the lasers should be enabled
	 */
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Enable or disable the lasers", Keywords = "Enable Disable"))
	void SetLasersEnabled(const bool Enabled);

	/// ABuildingBlockActorBase interface
	virtual void SetUpBuildingBlock(const class UBuildingBlockMeshComponent* BuildingBlockComponent) override;
	/// End of ABuildingBlockActorBase interface

protected:
	virtual void BeginPlay() override;

private:
	/** Callback for laser overlap */
	UFUNCTION()
	void OnLaserAreaBeginOverlap(UPrimitiveComponent* OverlappingComponent, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	/** Add lasers to this object based on NumberOfLasers */
	void AddLasers();

public:
	/** The number of lasers to place vertically inside the laser bounding box */
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The number of lasers to place vertically inside the laser bounding box"))
	uint8 NumberOfLasers = 6;

	/** The mesh to use for the laser beam */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh to use for the laser beam"))
	UStaticMesh* LaserBeamMesh;

	/** The mesh to use on either side of the laser beam */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh to use on either side of the laser beam"))
	UStaticMesh* LaserEndMesh;

	/** The area in which the lasers are placed. If the player enters this area, the alarm is triggered. */
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The area in which the lasers are placed. If the player enters this area, the alarm is triggered."))
	UBoxComponent* LaserArea;

private:
	/** The player's pawn */
	APawn* PlayerPawn;
	/** The alarm component for this actor */
	UAlarmComponent* AlarmComponent;

#if WITH_EDITOR
	/** An array of laser meshes added to this actor */
	TArray<UStaticMeshComponent*> AddedLaserMeshes;
	/** An array of meshes placed at the end of lasers */
	TArray<UStaticMeshComponent*> AddedLaserEnds;
#endif
};
