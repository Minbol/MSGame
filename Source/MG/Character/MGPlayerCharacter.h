// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MGCharacter.h"
#include "AbilitySystem/MGAbilitySet.h"
#include "MGPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;

/**
 * Player-controlled character.
 *
 * Adds the third-person camera rig and wires the PlayerState's ASC into
 * PawnExtensionComponent on possession/replication.
 *
 * Input flow (Step 4):
 *   SetupPlayerInputComponent → UMGInputComponent binds native actions (Move, Look)
 *   and ability actions (tag → AbilityInputTagPressed/Released on the ASC).
 *   Tick → ASC::ProcessAbilityInput flushes the press/release queues.
 *
 * AbilitySet flow (Step 4):
 *   OnAbilitySystemInitialized → grants PawnData->AbilitySet to the ASC.
 *   OnAbilitySystemUninitialized → revokes granted handles.
 */
UCLASS()
class MG_API AMGPlayerCharacter : public AMGCharacter
{
	GENERATED_BODY()

public:
	AMGPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE USpringArmComponent* GetCameraBoom()   const { return CameraBoom; }
	FORCEINLINE UCameraComponent*    GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void InitializePlayerASC();

	// Called when PawnExtensionComponent broadcasts OnAbilitySystemInitialized.
	void OnAbilitySystemInitialized();

	// Called when PawnExtensionComponent broadcasts OnAbilitySystemUninitialized.
	void OnAbilitySystemUninitialized();

	// Binds input actions from PawnData->InputConfig.
	// Called from SetupPlayerInputComponent (fast path) and
	// OnAbilitySystemInitialized (fallback when PawnData arrives late).
	void BindInputActions();

	// Native input callbacks (bound in SetupPlayerInputComponent).
	void Input_Move(const struct FInputActionValue& Value);
	void Input_MoveStarted(const struct FInputActionValue& Value);
	void Input_Look(const struct FInputActionValue& Value);

	// GAS input callbacks — routed through the ASC via InputTag.
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	// Set in BP class defaults (e.g. IMC_Default). Added at priority 0 on possession.
	UPROPERTY(EditDefaultsOnly, Category="MG|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MG|Components", meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MG|Components", meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent> FollowCamera;

	// Handles returned when the AbilitySet is granted — used to revoke on uninit.
	FMGAbilitySet_GrantedHandles AbilitySetHandles;

	// EnhancedInput binding handles for ability actions (kept to remove them on uninit).
	TArray<uint32> AbilityInputBindingHandles;
};
