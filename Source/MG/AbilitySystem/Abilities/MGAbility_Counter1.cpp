// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_Counter1.h"
#include "MGGameplayTags.h"

UMGAbility_Counter1::UMGAbility_Counter1(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ChainAbilityTag = MGGameplayTags::AbilityTag_ChainSkill_Counter1;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_ChainSkill_Counter1));

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_Counter1);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_Counter1);
}
