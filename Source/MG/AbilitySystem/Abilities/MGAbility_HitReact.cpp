// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_HitReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Attributes/MGHealthSet.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "MGGameplayTags.h"

UMGAbility_HitReact::UMGAbility_HitReact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 이미 피격 반응 중에도 새로운 MG.Event.Hit 이 들어오면 다시 트리거되어야 합니다
	// (기본값 false 라 연타로 맞아도 어빌리티가 재활성화되지 않고 무시됐었음).
	bRetriggerInstancedAbility = true;

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

	const float DamageMultiplier = TriggerEventData ? TriggerEventData->EventMagnitude : 1.f;
	ApplyDamage(DamageMultiplier);

	// 이 피격으로 사망했다면 피격 몽타주를 재생하지 않고 바로 끝냅니다.
	// Slot 이 몽타주를 계속 점유하면 AnimGraph 의 Dead 스테이트가 화면에 나오지 못하기 때문입니다.
	UMGAbilitySystemComponent* ASC = GetMGAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(MGGameplayTags::StatusTag_Dead))
	{
		if (CurrentMontageTask)
		{
			CurrentMontageTask->EndTask();
			CurrentMontageTask = nullptr;
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	const bool bStrongHit = TriggerEventData && TriggerEventData->InstigatorTags.HasTagExact(MGGameplayTags::HitReactionTag_Strong);
	UAnimMontage* MontageToPlay = bStrongHit ? StrongHitMontage : WeakHitMontage;

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	// 연타로 재트리거된 경우, 이전 몽타주 태스크의 델리게이트가 뒤늦게 발화해서
	// 방금 새로 시작한 활성화를 끝내버리는 걸 막기 위해 먼저 정리합니다.
	if (CurrentMontageTask)
	{
		CurrentMontageTask->EndTask();
		CurrentMontageTask = nullptr;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay);
	Task->OnCompleted.AddDynamic(this, &UMGAbility_HitReact::OnMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UMGAbility_HitReact::OnMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UMGAbility_HitReact::OnMontageCancelled);
	CurrentMontageTask = Task;
	Task->ReadyForActivation();
}

void UMGAbility_HitReact::OnMontageEnded()
{
	CurrentMontageTask = nullptr;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMGAbility_HitReact::OnMontageCancelled()
{
	CurrentMontageTask = nullptr;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMGAbility_HitReact::ApplyDamage(float DamageMultiplier)
{
	UMGAbilitySystemComponent* ASC = GetMGAbilitySystemComponent();
	const UMGHealthSet* HealthSet = ASC ? ASC->GetSet<UMGHealthSet>() : nullptr;
	if (!ASC || !HealthSet)
	{
		return;
	}

	const float Damage = BaseDamage * DamageMultiplier;
	ASC->ApplyModToAttribute(UMGHealthSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage);

	const float RemainingHealth = HealthSet->GetHealth();

#if !UE_BUILD_SHIPPING
	if (GEngine)
	{
		const AActor* Avatar = ASC->GetAvatarActor();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
			FString::Printf(TEXT("[Damage] %s took %.1f damage (x%.2f) | HP: %.1f / %.1f"),
				Avatar ? *Avatar->GetName() : TEXT("Unknown"),
				Damage, DamageMultiplier, RemainingHealth, HealthSet->GetMaxHealth()));
	}
#endif

	if (RemainingHealth <= 0.f)
	{
		ASC->AddLooseGameplayTag(MGGameplayTags::StatusTag_Dead);
	}
}
