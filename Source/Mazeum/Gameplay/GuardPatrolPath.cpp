// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardPatrolPath.h"

UGuardPatrolPathComponent::UGuardPatrolPathComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    for (int32 pointIndex = 0; pointIndex < GetNumberOfSplinePoints(); ++pointIndex)
    {
        SetSplinePointType(pointIndex, ESplinePointType::Linear);
    }
}
