// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_ChargeSkillBase.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Animation/AnimInstance.h"

UMGAbility_ChargeSkillBase::UMGAbility_ChargeSkillBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGAbility_ChargeSkillBase::OnBeforeMontagePlayed()
{
	UAbilityTask_WaitInputRelease* ReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	if (ReleaseTask)
	{
		ReleaseTask->OnRelease.AddDynamic(this, &UMGAbility_ChargeSkillBase::OnInputReleased);
		ReleaseTask->ReadyForActivation();
	}
}

void UMGAbility_ChargeSkillBase::OnInputReleased(float TimeHeld)
{
	if (!MontageToPlay)
	{
		return;
	}

	UAnimInstance* AnimInstance = CurrentActorInfo ? CurrentActorInfo->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_JumpToSection(ReleaseSectionName, MontageToPlay);
	}
}
