// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SplineComponent.h"

#include "GuardPatrolPath.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class UNREALSFAS_API UGuardPatrolPathComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UGuardPatrolPathComponent();

public:
	/** The likelihood of this guard path to be chosen over another path. Higher means a bigger chance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (ToolTip = "The likelihood of this guard path to be chosen over another path. Higher means a bigger chance.", ClampMin = "1"))
	uint8 PathWeight = 1;

	/** The time it takes for the guard to move from the spline's start to its end */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing", meta = (ToolTip = "The time it takes for the guard to move from the spline's start to its end", ClampMin = "0.1"))
	float PathDuration = 10.0f;

	/** The time the guard waits before reversing on its path. If negative, the guard doesn't reverse, but instead closes the spline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing", meta = (ToolTip = "The time the guard waits before reversing on its path. If negative, the guard doesn't reverse, but instead closes the spline.", ClampMin = "-1.0"))
	float PathEndWaitTime = 2.5f;
};
