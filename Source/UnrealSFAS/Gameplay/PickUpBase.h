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
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

	virtual void Interact(const AActor* InstigatedBy = nullptr) override;

	UFUNCTION(BlueprintCallable, meta  = (ToolTip = "Pick this item up", DefaultToSelf="PickedUpBy"))
	void PickUpItem(const AActor* PickedUpBy);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Drop this item"))
	void DropItem();

	FORCEINLINE float GetItemWeight() const { return ItemWeight; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The weight of this item. A higher value means that the player moves slower when holding this item", ClampMin = "0.0", AllowPrivateAccess = "true"))
	float ItemWeight = 10.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The pick-up's mesh", AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	const AActor* ItemPickedUpByActor = nullptr;
};
