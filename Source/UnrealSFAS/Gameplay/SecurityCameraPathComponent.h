// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "SecurityCameraPathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALSFAS_API USecurityCameraPathComponent : public USplineComponent
{
	GENERATED_BODY()

public:	
	USecurityCameraPathComponent();

public:
	UPROPERTY(EditAnywhere, Category = Timing, meta = (ToolTip = "The time it takes for the camera to look at the spline from start to end", ClampMin = "0.1"))
	float SplinePathDuration = 10.0f;
	UPROPERTY(EditAnywhere, Category = Timing, meta = (ToolTip = "The time the camera waits before reversing on its path. If negative, the camera doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float SplineEndWaitTime = 2.5f;
};
