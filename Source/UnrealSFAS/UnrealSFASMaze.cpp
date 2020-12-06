// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASMaze.h"

#include "MapGrid.h"

// Sets default values
AUnrealSFASMaze::AUnrealSFASMaze()
{
 	// No need to tick the maze at this point
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AUnrealSFASMaze::BeginPlay()
{
	Super::BeginPlay();

	if (WallMesh)
	{
		const int32 mazeSize = 20;
		const float blockSize = 200.0f;
		const float blockWidth = 2.0f;
		const float blockHeight = 5.0f;
		const float blockZPos = 50.0f;

		FMapGrid maze(mazeSize);
		maze.SetRow(0,  0b11111111111111111111);
		maze.SetRow(1,  0b10100000000000000001);
		maze.SetRow(2,  0b10101010111111111101);
		maze.SetRow(3,  0b10101010001111000101);
		maze.SetRow(4,  0b10101010100001010001);
		maze.SetRow(5,  0b10101010111111111111);
		maze.SetRow(6,  0b10101010000000000001);
		maze.SetRow(7,  0b10001011111111111101);
		maze.SetRow(8,  0b11111000000000000101);
		maze.SetRow(9,  0b10001111111111111101);
		maze.SetRow(10, 0b10101111111111111001);
		maze.SetRow(11, 0b10100000000000001101);
		maze.SetRow(12, 0b10111011111111101001);
		maze.SetRow(13, 0b10100010000000101011);
		maze.SetRow(14, 0b10101110111110101001);
		maze.SetRow(15, 0b10100000100010101101);
		maze.SetRow(16, 0b10101110001110100001);
		maze.SetRow(17, 0b10111111111110111101);
		maze.SetRow(18, 0b10000000000000000001);
		maze.SetRow(19, 0b11111111111111111101);		

		float xPos = 0.0f;
		float yPos = 0.0f;
		FQuat worldRotation(FVector(0.0f, 0.0f, 1.0f), 0.0f);
		FVector worldScale(blockWidth, blockWidth, blockHeight);

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the maze as static mesh components attached to the root of this actor
		for (int32 i = 0; i < mazeSize; i++)
		{
			yPos = static_cast<float>(i - (mazeSize / 2)) * blockSize;

			for (int32 j = 0; j < mazeSize; j++)
			{
				xPos = static_cast<float>(j - (mazeSize / 2)) * blockSize;

				if (!maze.IsEmpty(j, i))
				{
					UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this);
					FVector worldPosition(xPos, yPos, blockZPos);
					FTransform worldXForm(worldRotation, worldPosition, worldScale);

					meshComponent->SetStaticMesh(WallMesh);
					meshComponent->SetWorldTransform(worldXForm);
					meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepWorldTransform);
					meshComponent->RegisterComponent();
				}
			}
		}

	}
}

