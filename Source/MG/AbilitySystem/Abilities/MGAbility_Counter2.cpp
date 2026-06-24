// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_Counter2.h"
#include "MGGameplayTags.h"

UMGAbility_Counter2::UMGAbility_Counter2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ChainAbilityTag = MGGameplayTags::AbilityTag_ChainSkill_Counter2;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_ChainSkill_Counter2));

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_Counter2);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_Counter2);
}
