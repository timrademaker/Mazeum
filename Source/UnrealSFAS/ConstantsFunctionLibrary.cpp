// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstantsFunctionLibrary.h"

FVector UConstantsFunctionLibrary::BlockDimensions = FVector(100.0f);
FVector UConstantsFunctionLibrary::BlockScale = FVector(1.0f);

FVector UConstantsFunctionLibrary::GetBlockSize()
{
    return BlockDimensions * BlockScale;
}
