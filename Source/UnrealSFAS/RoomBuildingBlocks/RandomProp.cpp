// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomProp.h"

#include "RandomPropComponent.h"

// Sets default values
ARandomProp::ARandomProp()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
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
	}

	if (shouldBeRotated)
	{
		SetActorRotation(FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
	}
}
