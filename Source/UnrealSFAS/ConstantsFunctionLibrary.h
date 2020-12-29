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
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scaled size of a single block"))
    static FVector GetBlockSize();

    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scale of the blocks"))
    static FVector GetBlockScale();

    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Tooltip = "Get the scaled size of a single vent block"))
    static FVector GetVentBlockSize();
	
private:
    static FVector BlockDimensions;
    static FVector BlockScale;
    static float VentHeight;
};
