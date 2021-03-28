// Fill out your copyright notice in the Description page of Project Settings.


#include "Lasers.h"

#include "AlarmComponent.h"
#include "../RoomBuildingBlocks/BuildingBlockMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

ALasers::ALasers()
{
	PrimaryActorTick.bCanEverTick = false;

	LaserArea = CreateDefaultSubobject<UBoxComponent>("LaserArea");
	LaserArea->SetGenerateOverlapEvents(true);
	LaserArea->SetBoxExtent(FVector(50.0f, 10.0f, 100.0f));

	RootComponent = LaserArea;

	AlarmComponent = CreateDefaultSubobject<UAlarmComponent>("Alarm");
}

void ALasers::OnConstruction(const FTransform& Transform)
{
	AddLasers();
}

void ALasers::SetLasersEnabled(const bool Enabled)
{
	LaserArea->SetGenerateOverlapEvents(Enabled);
	SetActorHiddenInGame(!Enabled);
}

void ALasers::SetUpBuildingBlock(const UBuildingBlockMeshComponent* BuildingBlockComponent)
{
	const UStaticMesh* mesh = BuildingBlockComponent->GetStaticMesh();
	if (mesh)
	{
		const FVector meshExtents = mesh->GetBounds().BoxExtent;
		LaserArea->SetBoxExtent(meshExtents);

		// Scale is ignored intentionally, as this is adjusted when this actor has been set up (in RoomTemplate::ConvertToRoom)
	}
}

void ALasers::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	AddLasers();

	FScriptDelegate overlapDelegate;
	overlapDelegate.BindUFunction(this, "OnLaserAreaBeginOverlap");
	LaserArea->OnComponentBeginOverlap.Add(overlapDelegate);
}

void ALasers::OnLaserAreaBeginOverlap(UPrimitiveComponent* OverlappingComponent, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AActor>(OverlappedComponent) == PlayerPawn)
	{
		AlarmComponent->TriggerAlarm();
	}
}

void ALasers::AddLasers()
{
#if WITH_EDITOR
	for (UStaticMeshComponent* laser : AddedLaserMeshes)
	{
		laser->UnregisterComponent();
	}
	AddedLaserMeshes.Empty();

	for (UStaticMeshComponent* end : AddedLaserEnds)
	{
		end->UnregisterComponent();
	}
	AddedLaserEnds.Empty();
#endif

	if (NumberOfLasers == 0)
	{
		return;
	}

	const FVector laserAreaExtent = LaserArea->GetScaledBoxExtent();
	const float offsetPerLaser = (laserAreaExtent.Z * 2.0f) / static_cast<float>(NumberOfLasers);

	for (uint8 i = 0; i < NumberOfLasers; ++i)
	{
		const float laserZ = laserAreaExtent.Z - (offsetPerLaser / 2.0f) - (i * offsetPerLaser);

		if (LaserBeamMesh)
		{

			UStaticMeshComponent* laser = NewObject<UStaticMeshComponent>(this);
			laser->AttachToComponent(LaserArea, FAttachmentTransformRules::KeepRelativeTransform);
			laser->SetRelativeLocation(FVector(0.0f, 0.0f, laserZ));

			laser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			laser->SetStaticMesh(LaserBeamMesh);

			const float xScale = LaserArea->GetUnscaledBoxExtent().X / LaserBeamMesh->GetBounds().BoxExtent.X;
			laser->SetRelativeScale3D(FVector(xScale, 1.0f, 1.0f));

			laser->RegisterComponent();

#if WITH_EDITOR
			AddedLaserMeshes.Add(laser);
#endif
		}

		if (LaserEndMesh)
		{
			UStaticMeshComponent* leftLaserEnd = NewObject<UStaticMeshComponent>(this);

			leftLaserEnd->AttachToComponent(LaserArea, FAttachmentTransformRules::KeepRelativeTransform);

			leftLaserEnd->SetRelativeLocation(FVector(-laserAreaExtent.X, 0.0f, laserZ));

			leftLaserEnd->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			leftLaserEnd->SetStaticMesh(LaserEndMesh);

			UStaticMeshComponent* rightLaserEnd = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), NAME_None, RF_NoFlags, leftLaserEnd);
			rightLaserEnd->SetRelativeLocation(FVector(laserAreaExtent.X, 0.0f, laserZ));

			leftLaserEnd->RegisterComponent();
			rightLaserEnd->RegisterComponent();

#if WITH_EDITOR
			AddedLaserEnds.Add(leftLaserEnd);
			AddedLaserEnds.Add(rightLaserEnd);
#endif
		}
	}
}

