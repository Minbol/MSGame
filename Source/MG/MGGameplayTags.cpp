// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGGameplayTags.h"

namespace MGGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(InitState_Spawned,         "InitState.Spawned");
	UE_DEFINE_GAMEPLAY_TAG(InitState_DataAvailable,   "InitState.DataAvailable");
	UE_DEFINE_GAMEPLAY_TAG(InitState_DataInitialized, "InitState.DataInitialized");
	UE_DEFINE_GAMEPLAY_TAG(InitState_GameplayReady,   "InitState.GameplayReady");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move,         "MG.Input.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look,         "MG.Input.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint,       "MG.Input.Sprint");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump,         "MG.Input.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge,        "MG.Input.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Light, "MG.Input.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Heavy, "MG.Input.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Parry,        "MG.Input.Parry");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact,     "MG.Input.Interact");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_1,      "MG.Input.Skill.1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_2,      "MG.Input.Skill.2");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_3,      "MG.Input.Skill.3");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_4,      "MG.Input.Skill.4");

	UE_DEFINE_GAMEPLAY_TAG(AbilityTag_Jump,         "MG.Ability.Jump");
	UE_DEFINE_GAMEPLAY_TAG(AbilityTag_Dodge,        "MG.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(AbilityTag_Attack_Light, "MG.Ability.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(AbilityTag_Attack_Heavy, "MG.Ability.Attack.Heavy");

	UE_DEFINE_GAMEPLAY_TAG(StatusTag_Dead,       "MG.Status.Dead");
	UE_DEFINE_GAMEPLAY_TAG(StatusTag_Knockback,  "MG.Status.Knockback");
	UE_DEFINE_GAMEPLAY_TAG(StatusTag_Stagger,    "MG.Status.Stagger");
	UE_DEFINE_GAMEPLAY_TAG(StatusTag_Invincible, "MG.Status.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(StatusTag_Parrying,   "MG.Status.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(StatusTag_InAir,      "MG.Status.InAir");

	UE_DEFINE_GAMEPLAY_TAG(EventTag_Montage_Attack, "MG.Event.Montage.Attack");
	UE_DEFINE_GAMEPLAY_TAG(EventTag_Combo_Continue, "MG.Event.Combo.Continue");

	UE_DEFINE_GAMEPLAY_TAG(ComboAbilityTag_Attack_Light_01, "MG.Ability.Attack.Light.01");
	UE_DEFINE_GAMEPLAY_TAG(ComboAbilityTag_Attack_Heavy_01, "MG.Ability.Attack.Heavy.01");
}
