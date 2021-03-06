// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAZEUM_API IInteractableInterface
{
	GENERATED_BODY()

public:
    /**
     * Interact with this item
     * @param InstigatedBy The actor interacting with this item
     */
    virtual void Interact(const AActor* InstigatedBy = nullptr) = 0;
};
