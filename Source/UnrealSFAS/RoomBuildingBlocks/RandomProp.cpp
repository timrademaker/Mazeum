// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomProp.h"

#include "RandomPropComponent.h"

#include "Components/BoxComponent.h"

// Sets default values
ARandomProp::ARandomProp()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoxCollider->SetupAttachment(RootComponent);
}

void ARandomProp::SetUpBuildingBlock(const UBuildingBlockMeshComponent* BuildingBlockComponent)
{
	const URandomPropComponent* propComponent = Cast<URandomPropComponent>(BuildingBlockComponent);
	if(propComponent)
	{
		PropSize = propComponent->PropSize;
	}
}

// Called when the game starts or when spawned
void ARandomProp::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpProp();
}

void ARandomProp::SetUpProp()
{
	TArray<UStaticMesh*>* meshArrah = nullptr;
	bool shouldBeRotated = false;

	FRotator modelRotation;

	switch (PropSize)
	{
	case EPropSize::Small:
	{
		meshArrah = &SmallProps;
		shouldBeRotated = true;
		break;
	}
	case EPropSize::Medium:
	{
		meshArrah = &MediumProps;
		shouldBeRotated = true;
		break;
	}
	case EPropSize::Large:
	{
		meshArrah = &LargeProps;
		break;
	}
	}

	if (meshArrah->Num() > 0)
	{
		// Select random prop
		UStaticMesh* meshToUse = (*meshArrah)[FMath::RandRange(0, meshArrah->Num() - 1)];
		StaticMesh->SetStaticMesh(meshToUse);

		// Set up box collision
		const FVector meshExtent = StaticMesh->GetStaticMesh()->GetBounds().BoxExtent;
		BoxCollider->SetBoxExtent(meshExtent);
		BoxCollider->AddLocalOffset(FVector(0.0f, 0.0f, meshExtent.Z));
	}

	if (shouldBeRotated)
	{
		StaticMesh->AddRelativeRotation(FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
	}
}
