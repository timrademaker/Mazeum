// Fill out your copyright notice in the Description page of Project Settings.


#include "HandMadeMuseum.h"

#include "MapGrid.h"
#include "ProceduralGeneration/MuseumCeiling.h"
#include "ProceduralGeneration/MuseumFloor.h"
#include "ProceduralGeneration/MuseumVents.h"
#include "ProceduralGeneration/MuseumWalls.h"

void AHandMadeMuseum::BeginPlay()
{
	// Skip AMuseum's BeginPlay, but do call that of its parent
	Super::Super::BeginPlay();

	FMapGrid hallMask;
	FMapGrid ventEntranceMask;
	FMapGrid ventMask;
	FMapGrid doorMask;

	hallMask = FMapGrid(10, 11);
	hallMask.SetRow(3, 0b1100000000);
	hallMask.SetRow(4, 0b1111111111);
	hallMask.SetRow(5, 0b1111111111);
	hallMask.SetRow(6, 0b1111111111);
	hallMask.SetRow(7, 0b1100000000);


	FMapGrid ceilingMask = FMapGrid(10, 11);

	ceilingMask.SetRow(0, 0b0011111100);
	ceilingMask.SetRow(1, 0b0011111100);
	ceilingMask.SetRow(2, 0b0011111100);
	ceilingMask.SetRow(3, 0b1111111100);
	ceilingMask.SetRow(4, 0b1111111111);
	ceilingMask.SetRow(5, 0b1111111111);
	ceilingMask.SetRow(6, 0b1111111111);
	ceilingMask.SetRow(7, 0b1111111111);
	ceilingMask.SetRow(8, 0b0011111111);
	ceilingMask.SetRow(9, 0b0011111111);
	ceilingMask.SetRow(10, 0b0011111100);

	doorMask = FMapGrid(10, 11);
	doorMask.SetRow(3, 0b0001001000);
	doorMask.SetRow(7, 0b0001001010);

	ventMask = FMapGrid(10, 11);
	ventMask.SetRow(2, 0b0000000000);
	ventMask.SetRow(3, 0b0000001100);
	ventMask.SetRow(4, 0b0000011110);
	ventMask.SetRow(5, 0b0000010010);
	ventMask.SetRow(6, 0b0000010010);
	ventMask.SetRow(7, 0b0000111110);
	ventMask.SetRow(8, 0b0000100010);
	ventMask.SetRow(9, 0b0000111110);
	ventMask.SetRow(10, 0b0000000100);


	// Create floor actor
	AMuseumFloor* floor = GetWorld()->SpawnActor<AMuseumFloor>(MuseumFloorClass);
	floor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	floor->PlaceFloor(hallMask);

	// Create ceiling actor
	AMuseumCeiling* ceiling = GetWorld()->SpawnActor<AMuseumCeiling>(MuseumCeilingClass);
	ceiling->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	ceiling->PlaceCeiling(ceilingMask);

	// Create walls actor
	AMuseumWalls* walls = GetWorld()->SpawnActor<AMuseumWalls>(MuseumWallsClass);
	walls->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	walls->PlaceWalls(hallMask, doorMask);

	// Create vent actor
	AMuseumVents* vents = GetWorld()->SpawnActor<AMuseumVents>(MuseumVentsClass);
	vents->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	vents->PlaceVents(ventMask);
}