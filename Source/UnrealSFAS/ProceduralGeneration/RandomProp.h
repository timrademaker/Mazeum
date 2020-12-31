// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomProp.generated.h"

UENUM()
enum class EPropSize : uint8
{
	Small,
	Medium,
	Large
};

UCLASS()
class UNREALSFAS_API ARandomProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, meta = (ToolTip = "The size of the required prop"))
	EPropSize PropSize;

	UPROPERTY(EditDefaultsOnly, Category = Meshes, meta = (ToolTip = "Props that fit on the small pedestal"))
	TArray<UStaticMesh*> SmallProps;

	UPROPERTY(EditDefaultsOnly, Category = Meshes)
	TArray<UStaticMesh*> MediumProps;

	UPROPERTY(EditDefaultsOnly, Category = Meshes, meta = (ToolTip = "Props that fit on the wide table"))
	TArray<UStaticMesh*> LargeProps;

protected:
	/** The static mesh for this prop */
	UStaticMeshComponent* StaticMesh;
};
