// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = ItemMesh;
}

void APickUpBase::PickUpItem(const AActor* PickedUpBy)
{
	ItemPickedUpByActor = PickedUpBy;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void APickUpBase::DropItem()
{
	if (ItemPickedUpByActor)
	{
		const FVector itemMeshExtents = ItemMesh->GetStaticMesh()->GetBounds().BoxExtent;
		const FVector itemScale = ItemMesh->GetComponentScale();

		// Check where the item can be dropped
		FVector traceStart;
		FVector extentOfHoldingActor;
		ItemPickedUpByActor->GetActorBounds(true, traceStart, extentOfHoldingActor);

		const FVector traceEnd = traceStart - FVector(0.0f, 0.0f, extentOfHoldingActor.Z) - (ItemPickedUpByActor->GetActorForwardVector() * ((itemMeshExtents * itemScale * 2.0f) + extentOfHoldingActor));

		FHitResult hitResult;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(ItemPickedUpByActor);

		GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECollisionChannel::ECC_Camera, queryParams);

		// Drop the item (on the floor)
		if (hitResult.bBlockingHit)
		{
			FVector traceDirection = traceEnd - traceStart;
			traceDirection.Normalize();
			const FVector dropLocation = traceStart + (traceDirection * hitResult.Distance) + itemMeshExtents.Z * itemScale.Z;
			SetActorLocation(dropLocation, true);
		}
		else
		{
			SetActorLocation(traceEnd, true);
		}

		ItemPickedUpByActor = nullptr;
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}
