// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_HeavyAttack.h"
#include "MGGameplayTags.h"

UMGAbility_HeavyAttack::UMGAbility_HeavyAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityTags.AddTag(MGGameplayTags::AbilityTag_Attack_Heavy);

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_Attack_Heavy);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_InAir);
	// Parent tag MG.Ability.Attack matches both Attack.Light and Attack.Heavy on the ASC.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("MG.Ability.Attack")));
}
