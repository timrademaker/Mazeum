// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealSFASCharacter.generated.h"

UCLASS(config=Game)
class AUnrealSFASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUnrealSFASCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Called via input when the player tries to interact with something */
	void OnInteract();

	/** Called via input when the player tries to drop their held item */
	void OnDropHeldItem();

protected:
	// APawn interface
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	/**
	 * Calculate a movement speed modifier based on the item the player is currently holding
	 * @returns A normalized value (0-1) representing the movement speed modifier
	 */
	float CalculateMovementSpeedModifier() const;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** The range at which the player can interact with items like pick-ups and vents */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (ToolTip = "The range at which the player can interact with items", ClampMin = "0.0"))
	float InteractionRange = 150.0f;

	/** The minimum modifier to apply to the player's movement speed when an item is held */
	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (ToolTip = "The minimum movement speed modifier that can be applied to the player's movement speed when holding an item", ClampMin = "0.1", ClampMax = "1.0"))
	float MinimumMovementSpeedModifier = 0.5f;

	/** The minimum modifier to apply to the player's movement speed when an item is held */
	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (ToolTip = "The weight at which an item will slow down the player to a (near) standstill", ClampMin = "0.0"))
	float MaximumItemWeight = 100.0f;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Actors the player can interact with through the "Interact" keybind */
	TArray<AActor*> Interactables;

	/** The item the player is holding */
	class APickUpBase* HeldItem;
};

