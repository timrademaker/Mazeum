// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../InteractableInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS()
class UNREALSFAS_API APickUpBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
		
	DECLARE_MULTICAST_DELEGATE(FItemPickedUp);
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

	virtual void Interact(const AActor* InstigatedBy = nullptr) override;

	UFUNCTION(BlueprintCallable, meta  = (ToolTip = "Pick this item up", DefaultToSelf="PickedUpBy"))
	void PickUpItem(const AActor* PickedUpBy);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Drop this item"))
	void DropItem();

	/**
	 * Get the weight of this item
	 * @returns The weight of this item
	 */
	FORCEINLINE float GetItemWeight() const { return ItemWeight; }

	/** Delegate for when the item is picked up */
	FORCEINLINE FItemPickedUp& OnItemPickedUp() { return ItemPickedUpDelegate; }

private:
	/** The weight of this item, affecting the movement speed of anyone holding it */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The weight of this item. A higher value means that the player moves slower when holding this item", ClampMin = "0.0", AllowPrivateAccess = "true"))
	float ItemWeight = 10.0f;

	/** The mesh component for this actor */
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The pick-up's mesh", AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	/** The actor that is currently holding this item */
	const AActor* ItemPickedUpByActor = nullptr;

	/** The delegate that sends out an event when this item is picked up */
	FItemPickedUp ItemPickedUpDelegate;
};
