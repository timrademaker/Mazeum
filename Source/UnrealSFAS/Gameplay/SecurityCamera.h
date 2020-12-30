// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecurityCamera.generated.h"

class UDecalComponent;
class USplineComponent;
class UStaticMeshComponent;

class UAlarmComponent;

UCLASS()
class UNREALSFAS_API ASecurityCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Reset() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateCameraTargetPosition(float DeltaTime);
	/** Returns true if the camera is able to see the player */
	bool CameraHasSpottedPlayer();

public:
	UPROPERTY(EditAnywhere, Category="Timing", meta = (ToolTip = "The time it takes for the camera to look at the spline from start to end", ClampMin = "0.1"))
	float SplinePathDuration = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Timing", meta = (ToolTip = "The time the camera waits before reversing on its path. If negative, the camera doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float SplineEndWaitTime = 2.5f;
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The radius of the capsule which the camera uses as its vision capsule", ClampMin = "0.0"))
	float CameraVisionRadius = 50.0f;
	
	UPROPERTY(EditDefaultsOnly)
	UDecalComponent* CameraAreaDecal;
	UPROPERTY(EditDefaultsOnly)
	USplineComponent* PathSpline;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CameraMesh;	// Will probably be changed to a different type later on, if the camera needs to rotate
	

private:
	FCollisionShape VisionCollisionShape;
	FCollisionQueryParams CollisionQueryParams;

	APawn* PlayerPawn;
	UAlarmComponent* AlarmComponent;

	/// How far along the spline we are looking
	float CurrentSplineTime = 0.0f;
	/// How fast the camera should move along the spline, based on spline length and SplinePathDuration
	float CameraMovementSpeed;
	/// True if the camera is currently moving back to the spline's start
	bool IsMovingBackToSplineStart = false;
	/// How long the camera still has to wait before it starts to move in the opposite direction
	float ReverseCountDown;
};
