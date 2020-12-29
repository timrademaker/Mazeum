// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstantsFunctionLibrary.h"

FVector UConstantsFunctionLibrary::BlockDimensions = FVector(200.0f, 200.0f, 300.0f);
FVector UConstantsFunctionLibrary::BlockScale = FVector(1.0f);
FVector UConstantsFunctionLibrary::VentBlockScale = FVector(2.0f, 2.0f, 1.2f);
float UConstantsFunctionLibrary::VentHeight = 120.0f;

FVector UConstantsFunctionLibrary::GetBlockSize()
{
    return BlockDimensions * BlockScale;
}

FVector UConstantsFunctionLibrary::GetBlockScale()
{
    return BlockScale;
}

FVector UConstantsFunctionLibrary::GetVentBlockSize()
{
    return FVector(BlockDimensions.X, BlockDimensions.Y, VentHeight) * BlockScale;
}

FVector UConstantsFunctionLibrary::GetVentBlockScale()
{
    return VentBlockScale;
}
