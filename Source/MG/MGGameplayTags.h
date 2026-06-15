// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace MGGameplayTags
{
	// Init-state chain tags — registered in UMGGameInstance::Init()
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	// Native input tags (bound directly to character functions)
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);

	// Ability input tags (activate GAS abilities via ASC)
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Dodge);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Light);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Heavy);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Parry);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_1);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_2);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_3);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_4);

	// Ability self-tags — applied to the ASC while the ability is active (ActivationOwnedTags)
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Jump);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Dodge);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Attack_Light);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Attack_Heavy);

	// Status tags — set externally by GEs, damage system, or movement callbacks
	// Used in ActivationBlockedTags / ActivationRequiredTags
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Dead);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Knockback);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Stagger);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Invincible);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Parrying);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_InAir);

	// Event tags — sent via SendGameplayEventToActor for anim-notify hit detection
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Montage_Attack);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Combo_Continue);
}
