// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lasers.generated.h"

class UBoxComponent;
class UStaticMesh;

class UAlarmComponent;

UCLASS()
class UNREALSFAS_API ALasers : public AActor
{
	GENERATED_BODY()
	
public:	
	ALasers();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Enable or disable the lasers", Keywords = "Enable Disable"))
	void SetLasersEnabled(const bool Enabled);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnLaserAreaOverlap(UPrimitiveComponent* OverlappingComponent, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The number of lasers to place vertically inside the laser bounding box"))
	uint8 NumberOfLasers = 6;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh to use for the laser beam"))
	UStaticMesh* LaserBeamMesh;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The mesh to use on either side of the laser beam"))
	UStaticMesh* LaserEndMesh;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "The area in which the lasers are placed. If the player enters this area, the alarm is triggered."))
	UBoxComponent* LaserArea;

protected:
	bool LasersAreEnabled = true;

private:
	APawn* PlayerPawn;
	UAlarmComponent* AlarmComponent;

	TArray<UStaticMeshComponent*> AddedLaserMeshes;
#if WITH_EDITOR
	TArray<UStaticMeshComponent*> AddedLaserEnds;
#endif

};
