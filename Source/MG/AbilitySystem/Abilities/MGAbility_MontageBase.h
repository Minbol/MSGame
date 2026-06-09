// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGGameplayAbility.h"
#include "MGAbility_MontageBase.generated.h"

/**
 * Abstract base for montage-driven abilities (attacks, dodge, parry, skills, etc.).
 *
 * Flow:
 *   ActivateAbility
 *     → CommitAbility (cost / cooldown gate)
 *     → OnBeforeMontagePlayed()     ← subclass hook for pre-montage work
 *     → PlayMontageAndWait task     (ends ability on complete / interrupted / cancelled)
 *
 * If MontageToPlay is null the ability ends immediately after OnBeforeMontagePlayed().
 * Subclasses only need to set tags in their constructor and override OnBeforeMontagePlayed()
 * when extra logic is required (e.g. LaunchCharacter for dodge).
 */
UCLASS(Abstract)
class MG_API UMGAbility_MontageBase : public UMGGameplayAbility
{
	GENERATED_BODY()

public:
	UMGAbility_MontageBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Montage to play. Set on the BP subclass. Null = ability ends immediately after the hook.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Ability")
	TObjectPtr<UAnimMontage> MontageToPlay;

protected:
	// Override to run logic between CommitAbility and montage start (e.g. impulse, state changes).
	// Call EndAbility inside this function to abort before the montage plays.
	virtual void OnBeforeMontagePlayed() {}

private:
	UFUNCTION()
	void OnMontageEnded();

	UFUNCTION()
	void OnMontageCancelled();
};
