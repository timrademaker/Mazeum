// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardPathActor.h"

#include "GuardPatrolPath.h"
#include "../StealthGameMode.h"

// Sets default values
AGuardPathActor::AGuardPathActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GuardPathComponent = CreateDefaultSubobject<UGuardPatrolPathComponent>(TEXT("PatrolPath"));
	GuardPathComponent->SetupAttachment(RootComponent);
	
	GuardPathArray.Add(GuardPathComponent);
}

// Called when the game starts or when spawned
void AGuardPathActor::BeginPlay()
{
	Super::BeginPlay();
	
	AStealthGameMode* gm = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		gm->RegisterGuardPatrolPathArray(&GuardPathArray);
	}
}
