// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_LightAttack.h"
#include "MGGameplayTags.h"

UMGAbility_LightAttack::UMGAbility_LightAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityTags.AddTag(MGGameplayTags::AbilityTag_Attack_Light);

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_Attack_Light);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	// Parent tag MG.Ability.Attack matches both Attack.Light and Attack.Heavy on the ASC.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("MG.Ability.Attack")));
}
