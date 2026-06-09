// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UMGAbility_MontageBase::UMGAbility_MontageBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UMGAbility_MontageBase::ActivateAbility(FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	OnBeforeMontagePlayed();

	// Hook may have ended the ability early (e.g. missing character reference).
	if (!IsActive())
	{
		return;
	}

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay);
	Task->OnCompleted.AddDynamic(this, &UMGAbility_MontageBase::OnMontageEnded);
	Task->OnBlendOut.AddDynamic(this, &UMGAbility_MontageBase::OnMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UMGAbility_MontageBase::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UMGAbility_MontageBase::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UMGAbility_MontageBase::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMGAbility_MontageBase::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
