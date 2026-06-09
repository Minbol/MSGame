// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "MGPlayerState.generated.h"

class UMGAbilitySystemComponent;
class UMGHealthSet;
class UMGStaminaSet;
class UMGCombatSet;

/**
 * Owns the ASC and AttributeSets for player-controlled pawns.
 * AI enemies own their own ASC directly on the pawn (step 3).
 *
 * Network pattern:
 *   Server  — InitAbilityActorInfo called in AMGCharacter::PossessedBy
 *   Client  — InitAbilityActorInfo called in AMGCharacter::OnRep_PlayerState
 */
UCLASS()
class MG_API AMGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMGPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "MG|PlayerState")
	UMGAbilitySystemComponent* GetMGAbilitySystemComponent() const { return AbilitySystemComponent; }

	template <class T>
	const T* GetAttributeSet() const
	{
		return Cast<T>(GetAbilitySystemComponent()->GetAttributeSet(T::StaticClass()));
	}

private:
	UPROPERTY(VisibleAnywhere, Category = "MG|PlayerState")
	TObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;

	// AttributeSets are owned by this PlayerState, registered with the ASC in the constructor.
	UPROPERTY()
	TObjectPtr<UMGHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UMGStaminaSet> StaminaSet;

	UPROPERTY()
	TObjectPtr<UMGCombatSet> CombatSet;
};
