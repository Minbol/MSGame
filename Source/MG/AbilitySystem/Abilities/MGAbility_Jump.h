// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGGameplayAbility.h"
#include "MGAbility_Jump.generated.h"

/**
 * Triggers ACharacter::Jump via GAS. Ends immediately — animation is driven by the movement component.
 *
 * CanActivateAbility additionally checks Character::CanJump() so this ability naturally gates
 * double-jump attempts when not grounded (until a double-jump feature is added later).
 */
UCLASS()
class MG_API UMGAbility_Jump : public UMGGameplayAbility
{
	GENERATED_BODY()

public:
	UMGAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CanActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
