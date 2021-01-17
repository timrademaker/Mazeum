// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../RoomBuildingBlocks/BuildingBlockActorBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecurityCamera.generated.h"

UCLASS()
class UNREALSFAS_API ASecurityCamera : public ABuildingBlockActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecurityCamera();

	/// ABuildingBlockActorBase interface
	virtual void SetUpBuildingBlock(const class UBuildingBlockMeshComponent* BuildingBlockComponent) override;
	/// End of ABuildingBlockActorBase interface

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Called when the level is reset */
	virtual void Reset() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** 
	 * Update what the camera is looking at
	 * @param DeltaTime The time since the last camera update
	 */
	void UpdateCameraTargetPosition(float DeltaTime);

	/** Check if the camera is able to see an actor 
	 * @param Actor The actor for which to check if the camera is able to see it
	 * @returns True if the camera is able to see the actor
	 */
	bool CameraCanSeeActor(const AActor* Actor);

public:
	/** How long (in seconds) the player can stay in the camera's view before the alarm is raised */
	UPROPERTY(EditAnywhere, Category = Timing, meta = (ToolTip = "How long (in seconds) the player can stay in the camera's view before the alarm is raised", ClampMin = "0.0"))
	float MaximumTimeInView = 0.05f;
	/** The camera's field of view in degrees */
	UPROPERTY(EditAnywhere, Category = Vision, meta = (ToolTip = "The camera's field of view in degrees", ClampMin = "0.0", ClampMax = "180.0"))
	float FieldOfView = 15.0f;
	
	/** The decal that indicates where the camera is looking */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The decal that indicates where the camera is looking"))
	UDecalComponent* CameraAreaDecal;
	/** The path this camera follows */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The path this camera follows"))
	class USecurityCameraPathComponent* CameraPath;
	/** The camera's static mesh component */
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CameraMesh;	// Will probably be changed to a different type later on, if the camera needs to rotate
	/** The camera's spotlight, indicating where the camera is looking */
	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* SpotLight;
	
private:
	/** The player's pawn */
	APawn* PlayerPawn = nullptr;
	/** The camera's alarm component */
	class UAlarmComponent* AlarmComponent = nullptr;

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

	/** How long the player has been in view consecutively */
	float CurrentTimeInView = 0.0f;
};
