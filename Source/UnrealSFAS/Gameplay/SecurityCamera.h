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
	/** Update what the camera is looking at */
	void UpdateCameraTargetPosition(float DeltaTime);
	/** Check if the camera is able to see an actor 
	 * @param Actor The actor for which to check if the camera is able to see it
	 * @returns True if the camera is able to see the actor
	 */
	bool CameraCanSeeActor(const AActor* Actor);

public:
	UPROPERTY(EditAnywhere, Category="Timing", meta = (ToolTip = "The time it takes for the camera to look at the spline from start to end", ClampMin = "0.1"))
	float SplinePathDuration = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Timing", meta = (ToolTip = "The time the camera waits before reversing on its path. If negative, the camera doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float SplineEndWaitTime = 2.5f;
	UPROPERTY(EditAnywhere, Category = "Vision", meta = (ToolTip = "The camera's field of view in degrees", ClampMin = "0.0", ClampMax = "180.0"))
	float FieldOfView = 15.0f;
	
	UPROPERTY(EditDefaultsOnly)
	UDecalComponent* CameraAreaDecal;
	UPROPERTY(EditDefaultsOnly)
	USplineComponent* PathSpline;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CameraMesh;	// Will probably be changed to a different type later on, if the camera needs to rotate
	
private:
	/** The player's pawn */
	APawn* PlayerPawn = nullptr;
	/** The camera's alarm component */
	UAlarmComponent* AlarmComponent = nullptr;

	/** The cosine of half of the camera's field of view, used to determine if the camera can see an actor */
	float HalfFieldOfViewCosine = 0.0f;
	/** The tangent of half of the camera's field of view, used to determine the size of the decal */
	float HalfFieldOfViewTangent = 0.0f;
	/** The parameters used to check if an actor can be seen */
	FCollisionQueryParams CollisionQueryParams;
	/** The current range of the camera (which is based on the distance to the target position on the spline) */
	float CurrentCameraRange = 0.0f;

	/** How far along the spline the camera is looking */
	float CurrentDistanceAlongSpline = 0.0f;
	/** The length of the camera's path spline */
	float SplineLength = 0.0f;
	/** How fast the camera should move along the spline, based on spline length and SplinePathDuration */
	float CameraMovementSpeed = 0.0f;
	/** True if the camera is currently moving back to the spline's start */
	bool IsMovingBackToSplineStart = false;
	/** How long the camera still has to wait before it starts to move in the opposite direction */
	float ReverseCountDown = 0.0f;
};
