// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTemplate.generated.h"

class UBoxComponent;

UCLASS()
class UNREALSFAS_API ARoomTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomTemplate();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The size of the room template in tiles"))
	FIntVector RoomSize = FIntVector(5, 5, 5);

private:
	UBoxComponent* RoomBounds;

};
