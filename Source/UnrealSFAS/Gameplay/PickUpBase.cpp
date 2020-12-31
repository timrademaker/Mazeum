// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PropSize = EPropSize::Small;
}

void APickUpBase::Interact(const AActor* InstigatedBy)
{
	PickUpItem(InstigatedBy);
}

void APickUpBase::PickUpItem(const AActor* PickedUpBy)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	ItemPickedUpDelegate.Broadcast();
}

void APickUpBase::BeginPlay()
{
	ARandomProp* randomProp = Cast<ARandomProp>(RandomPropBlueprint);

	if (randomProp && SmallProps.Num() == 0 && MediumProps.Num() == 0 && LargeProps.Num() == 0)
	{
		SmallProps = randomProp->SmallProps;
		MediumProps = randomProp->MediumProps;
		LargeProps = randomProp->LargeProps;
	}

	Super::BeginPlay();
}
