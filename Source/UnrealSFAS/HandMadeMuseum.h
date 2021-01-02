// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Museum.h"
#include "HandMadeMuseum.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API AHandMadeMuseum : public AMuseum
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
