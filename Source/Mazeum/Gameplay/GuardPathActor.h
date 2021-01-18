// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuardPathActor.generated.h"

UCLASS()
class MAZEUM_API AGuardPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuardPathActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	class UGuardPatrolPathComponent* GuardPathComponent;

	/** The "array" of guard patrol paths that is registered to the GameMode */
	TArray<class UGuardPatrolPathComponent*> GuardPathArray;

};
