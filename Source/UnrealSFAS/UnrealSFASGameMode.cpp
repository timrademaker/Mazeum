// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSFASGameMode.h"
#include "UnrealSFASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealSFASGameMode::AUnrealSFASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
