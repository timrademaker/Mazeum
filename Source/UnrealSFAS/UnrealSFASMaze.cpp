// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASMaze.h"

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

		// Array of binary values: 1 = wall, 0 = space
		uint32 mazeArray[mazeSize] = {	0b11111111111111111111,
										0b10100000000000000001,
										0b10101010111111111101,
										0b10101010001111000101,
										0b10101010100001010001,
										0b10101010111111111111,
										0b10101010000000000001,
										0b10001011111111111101,
										0b11111000000000000101,
										0b10001111111111111101,
										0b10101111111111111001,
										0b10100000000000001101,
										0b10111011111111101001,
										0b10100010000000101011,
										0b10101110111110101001,
										0b10100000100010101101,
										0b10101110001110100001,
										0b10111111111110111101,
										0b10000000000000000001,
										0b11111111111111111101 };

		float xPos = 0.0f;
		float yPos = 0.0f;
		FQuat worldRotation(FVector(0.0f, 0.0f, 1.0f), 0.0f);
		FVector worldScale(blockWidth, blockWidth, blockHeight);
		uint32 mazeRow;

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the maze as static mesh components attached to the root of this actor
		for (int32 i = 0; i < mazeSize; i++)
		{
			yPos = static_cast<float>(i - (mazeSize / 2)) * blockSize;
			mazeRow = mazeArray[i];

			for (int32 j = 0; j < mazeSize; j++)
			{
				xPos = static_cast<float>(j - (mazeSize / 2)) * blockSize;

				uint32 mazeValue = (mazeRow >> (mazeSize - (j + 1))) & 1;

				if (mazeValue)
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

