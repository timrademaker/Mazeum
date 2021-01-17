// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBlockMeshComponent.h"
#include "SecurityCameraBuildingBlock.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API USecurityCameraBuildingBlock : public UBuildingBlockMeshComponent
{
	GENERATED_BODY()
	
public:
	/** The time it takes for the camera to look at the spline from start to end */
	UPROPERTY(EditAnywhere, Category = Timing, meta = (ToolTip = "The time it takes for the camera to look at the spline from start to end", ClampMin = "0.1"))
	float SplinePathDuration = 10.0f;
	/** The time the camera waits before reversing on its path. If negative, the camera doesn't reverse, but instead closes the spline. */
	UPROPERTY(EditAnywhere, Category = Timing, meta = (ToolTip = "The time the camera waits before reversing on its path. If negative, the camera doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float SplineEndWaitTime = 2.5f;
};
