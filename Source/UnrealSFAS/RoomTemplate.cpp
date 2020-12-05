// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplate.h"

#include "Components/BoxComponent.h"

// Sets default values
ARoomTemplate::ARoomTemplate()
{
 	// No need to tick this actor
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* rootComponent = GetRootComponent();

	// Add a root component we can move the bounding box relative to
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	root->AttachToComponent(rootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	root->RegisterComponent();
	
	// Create bounding box
	RoomBounds = CreateDefaultSubobject<UBoxComponent>("Room outline");
	RoomBounds->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	RoomBounds->RegisterComponent();
}

void ARoomTemplate::OnConstruction(const FTransform& Transform)
{
	// TODO: Move this to a place with constants
	const FVector blockDimensions(100.0f, 100.0f, 100.0f);
	
	const FVector newExtent = FVector(RoomSize) * blockDimensions * 0.5f;
	RoomBounds->SetBoxExtent(newExtent, false);
	RoomBounds->SetRelativeLocation(newExtent);
}

// Called when the game starts or when spawned
void ARoomTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}

