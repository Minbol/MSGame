// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_ChainSkillBase.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "MGGameplayTags.h"

UMGAbility_ChainSkillBase::UMGAbility_ChainSkillBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGAbility_ChainSkillBase::OnBeforeMontagePlayed()
{
	// CommitAbility 통과 후 몽타주 직전에 등록 — 이벤트를 절대 놓치지 않습니다.
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, MGGameplayTags::EventTag_ChainSkill_Window);

	if (Task)
	{
		Task->EventReceived.AddDynamic(this, &UMGAbility_ChainSkillBase::OnChainSkillWindowEvent);
		Task->ReadyForActivation();
	}
}

void UMGAbility_ChainSkillBase::OnChainSkillWindowEvent(FGameplayEventData Payload)
{
	UMGAbilitySystemComponent* MGASC = GetMGAbilitySystemComponent();
	if (!MGASC || !ChainAbilityTag.IsValid())
	{
		return;
	}

	if (!MGASC->HasChainSkillInput(ChainAbilityTag))
	{
		return;
	}

	FGameplayTag CachedChainTag = ChainAbilityTag;

	// EndAbility 로 현재 몽타주 정지를 요청하고, 다음 틱에 연계기를 활성화합니다.
	// (같은 프레임 활성화 시 슬롯 충돌 방지)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	TWeakObjectPtr<UMGAbilitySystemComponent> WeakASC(MGASC);
	if (UWorld* World = MGASC->GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateLambda([WeakASC, CachedChainTag]()
			{
				if (UMGAbilitySystemComponent* ASC = WeakASC.Get())
				{
					ASC->TryActivateChainSkill(CachedChainTag);
				}
			})
		);
	}
}
