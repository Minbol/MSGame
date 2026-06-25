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
	// 연계기 전용 홀드 입력 (MG.Input.Attack 부모에서 분리 → 콤보 버퍼에 걸리지 않음)
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ChainSkill_Left_Hold);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ChainSkill_Right_Hold);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Parry);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_1);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_2);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_3);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_4);

	// Ability self-tags — applied to the ASC while the ability is active (ActivationOwnedTags)
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Jump);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Dodge);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Attack);           // 부모 태그: MG.Ability.Attack.*
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Attack_Light);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_Attack_Heavy);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_ChainSkill);       // 부모 태그: MG.Ability.ChainSkill.*
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_ChainSkill_Counter1);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_ChainSkill_Counter2);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_ChainSkill_ChainReflection1);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityTag_ChainSkill_ChainReflection2);

	// 연계기 윈도우 이벤트 — 몽타주 AnimNotify 에서 이 태그로 SendGameplayEvent 를 호출합니다.
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_ChainSkill_Window);

	// Combo step tags — used as ComboAbilityTag (key in UMGComboTableDataAsset).
	// 1단계는 C++ 선언, 이후 단계는 DefaultGameplayTags.ini 에 추가하세요.
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboAbilityTag_Attack_Light_01);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboAbilityTag_Attack_Heavy_01);

	// Status tags — set externally by GEs, damage system, or movement callbacks
	// Used in ActivationBlockedTags / ActivationRequiredTags
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Dead);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Knockback);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Stagger);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Invincible);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_Parrying);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_InAir);
	// 이동 입력으로 몽타주를 캔슬할 수 있는 창이 열려 있음을 나타내는 태그
	// AnimNotifyState_MontageCancel 이 Begin/End 에서 추가/제거합니다.
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusTag_MoveCancelWindow);

	// Event tags — sent via SendGameplayEventToActor for anim-notify hit detection
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Montage_Attack);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Combo_Continue);
	MG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Hit);            // 무기 스윕으로 피격 시 대상에게 전송
}
