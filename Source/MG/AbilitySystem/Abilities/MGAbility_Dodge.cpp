// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "Character/MGCharacter.h"
#include "MGGameplayTags.h"

UMGAbility_Dodge::UMGAbility_Dodge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_Dodge));

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_Dodge);
	ActivationOwnedTags.AddTag(MGGameplayTags::StatusTag_Invincible);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Stagger);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_InAir);
	ActivationBlockedTags.AddTag(MGGameplayTags::AbilityTag_Dodge);

	// 연계기 테이블에서 이 키로 역조(Counter)를 조회합니다.
	ChainAbilityTag = MGGameplayTags::AbilityTag_Dodge;
}

void UMGAbility_Dodge::OnBeforeMontagePlayed()
{
	// 연계 윈도우 이벤트 리스너 등록 (MG.Event.ChainSkill.Window 대기)
	Super::OnBeforeMontagePlayed();

	AMGCharacter* Character = GetMGCharacterFromActorInfo();
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	FVector DodgeDir = Character->GetLastMovementInputVector().GetSafeNormal2D();
	if (DodgeDir.IsNearlyZero())
	{
		DodgeDir = Character->GetActorForwardVector().GetSafeNormal2D();
	}

	Character->LaunchCharacter(DodgeDir * DodgeImpulse, /*bXYOverride=*/true, /*bZOverride=*/false);
}
