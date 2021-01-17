// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConstantsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UConstantsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /** Get the scaled size of a single block */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scaled size of a single block"))
    static FVector GetBlockSize();

    /** Get the scale of the blocks */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scale of the blocks"))
    static FVector GetBlockScale();

    /** Get the scaled size of a single vent block */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scaled size of a single vent block"))
    static FVector GetVentBlockSize();

    /** Get the scale of the vent blocks */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scale of the vent blocks"))
    static FVector GetVentBlockScale();
	
private:
    /** The dimensions of a single building block */
    static FVector BlockDimensions;
    /** The scale of each building block */
    static FVector BlockScale;
    /** The scale of each vent block */
    static FVector VentBlockScale;
    /** The height of each vent block */
    static float VentHeight;
};
