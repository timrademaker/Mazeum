// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GuardPatrolPath.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct UNREALSFAS_API FGuardPatrolPath
{
	GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ToolTip = "The spline the guard follows"))
	USplineComponent* PathSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ToolTip = "The likelihood of this guard path to be chosen over another path. Higher means a bigger chance.", ClampMin = "1"))
	uint8 PathWeight = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing", meta = (ToolTip = "The time it takes for the guard to move from the spline's start to its end", ClampMin = "0.1"))
	float PathDuration = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing", meta = (ToolTip = "The time the guard waits before reversing on its path. If negative, the guard doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float PathEndWaitTime = 2.5f;
};
