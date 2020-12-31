// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../InteractableInterface.h"
#include "../ProceduralGeneration/RandomProp.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS()
class UNREALSFAS_API APickUpBase : public ARandomProp, public IInteractableInterface
{
	GENERATED_BODY()
		
	DECLARE_MULTICAST_DELEGATE(FItemPickedUp);
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

	virtual void Interact(const AActor* InstigatedBy = nullptr) override;

	UFUNCTION(BlueprintCallable, meta  = (ToolTip = "Pick this item up", DefaultToSelf="PickedUpBy"))
	void PickUpItem(const AActor* PickedUpBy);

	/** Delegate for when the item is picked up */
	FORCEINLINE FItemPickedUp& OnItemPickedUp() { return ItemPickedUpDelegate; }

protected:
	virtual void BeginPlay() override;

public:
	/** The blueprint class for RandomProp, where random props are set */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARandomProp> RandomPropBlueprint;

private:
	/** The delegate that sends out an event when this item is picked up */
	FItemPickedUp ItemPickedUpDelegate;
};
