// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_HitReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MGGameplayTags.h"

UMGAbility_HitReact::UMGAbility_HitReact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_HitReact));

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);

	// MG.Event.Hit 이벤트 수신 시 입력 없이 자동으로 활성화됩니다.
	FAbilityTriggerData HitTrigger;
	HitTrigger.TriggerTag = MGGameplayTags::EventTag_Hit;
	HitTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(HitTrigger);
}

void UMGAbility_HitReact::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const bool bStrongHit = TriggerEventData && TriggerEventData->InstigatorTags.HasTagExact(MGGameplayTags::HitReactionTag_Strong);
	UAnimMontage* MontageToPlay = bStrongHit ? StrongHitMontage : WeakHitMontage;

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay);
	Task->OnCompleted.AddDynamic(this, &UMGAbility_HitReact::OnMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UMGAbility_HitReact::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UMGAbility_HitReact::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UMGAbility_HitReact::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMGAbility_HitReact::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
