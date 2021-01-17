// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../InteractableInterface.h"
#include "../RoomBuildingBlocks/RandomProp.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS()
class UNREALSFAS_API APickUpBase : public ARandomProp, public IInteractableInterface
{
	GENERATED_BODY()
		
	DECLARE_EVENT(APickUpBase, FItemPickedUp);
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

	// IInteractableInterface interface
	virtual void Interact(const AActor* InstigatedBy = nullptr) override;
	// Enf of IInteractableInterface interface

	/**
	 * Pick up this item
	 * @param PickedUpBy The actor picking up this item
	 */
	UFUNCTION(BlueprintCallable, meta  = (ToolTip = "Pick this item up", DefaultToSelf="PickedUpBy"))
	void PickUpItem(const AActor* PickedUpBy);

	/** Get the event that is broadcast when the item is picked up */
	FORCEINLINE FItemPickedUp& OnItemPickedUp() { return ItemPickedUpEvent; }

protected:
	virtual void BeginPlay() override;

public:
	/** The blueprint class for RandomProp, where random props are set */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARandomProp> RandomPropBlueprint;

private:
	/** The event that sends out an event when this item is picked up */
	FItemPickedUp ItemPickedUpEvent;
};
