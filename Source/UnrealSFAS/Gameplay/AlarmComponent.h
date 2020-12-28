// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlarmComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALSFAS_API UAlarmComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FAlarmTriggered);

public:	
	// Sets default values for this component's properties
	UAlarmComponent();

	/** Trigger the alarm and send out an event */
	UFUNCTION(BlueprintCallable)
	void TriggerAlarm() const { AlarmTriggeredDelegate.Broadcast(); }

	/** Delegate for when the alarm is triggered */
	FORCEINLINE FAlarmTriggered& OnAlarmTriggered() { return AlarmTriggeredDelegate; }


private:
	FAlarmTriggered AlarmTriggeredDelegate;
};
