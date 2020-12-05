// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "DoorMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UDoorMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The actor equivalent of this component. In the case of a door mesh, this would be the actual door."))
    TSubclassOf<AActor> DoorActor;
};
