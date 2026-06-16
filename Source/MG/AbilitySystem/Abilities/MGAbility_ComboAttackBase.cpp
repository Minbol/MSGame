// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_ComboAttackBase.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "MGGameplayTags.h"

UMGAbility_ComboAttackBase::UMGAbility_ComboAttackBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 콤보 스텝 BP들이 별도 설정 없이도 아래 두 가지를 보장합니다:
	// 1) ASC 에 MG.Ability.Attack 태그가 올라가 → AbilityInputTagPressed 가 입력을 콤보 버퍼로 라우팅
	// 2) 다른 공격 어빌리티가 이 능력 실행 중 끼어들지 못하도록 차단
	static const FGameplayTag AttackParentTag = FGameplayTag::RequestGameplayTag(FName("MG.Ability.Attack"));
	ActivationOwnedTags.AddTag(AttackParentTag);
	ActivationBlockedTags.AddTag(AttackParentTag);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
}

void UMGAbility_ComboAttackBase::OnBeforeMontagePlayed()
{
	// CommitAbility 통과 후 몽타주 직전에 등록 — 이벤트를 절대 놓치지 않습니다.
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, MGGameplayTags::EventTag_Combo_Continue);
	
	if (Task)
	{
		Task->EventReceived.AddDynamic(this, &UMGAbility_ComboAttackBase::OnComboWindowEvent);
		Task->ReadyForActivation();
	}
}

void UMGAbility_ComboAttackBase::OnComboWindowEvent(FGameplayEventData Payload)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("[ComboWindow] OnComboWindowEvent called"));
	}

	UMGAbilitySystemComponent* MGASC = GetMGAbilitySystemComponent();
	if (!MGASC || !ComboAbilityTag.IsValid())
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("[ComboWindow] MGASC or ComboAbilityTag invalid"));
		return;
	}

	if (!MGASC->HasPendingComboInput())
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("[ComboWindow] No pending input"));
		return;
	}

	FGameplayTag CachedComboTag = ComboAbilityTag;

	// EndAbility 로 현재 몽타주 정지를 요청하지만, 애니메이션 시스템은 해당 프레임 말에
	// 정지를 처리합니다. 같은 프레임에 TryActivatePendingCombo 를 호출하면 다음 몽타주가
	// 전 몽타주와 슬롯 충돌을 일으켜 PlayMontageAndWait 가 즉시 OnInterrupted 를 발동,
	// 2nd 어빌리티가 조기 종료됩니다. 다음 틱으로 미뤄 충돌을 피합니다.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	TWeakObjectPtr<UMGAbilitySystemComponent> WeakASC(MGASC);
	if (UWorld* World = MGASC->GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateLambda([WeakASC, CachedComboTag]()
			{
				if (UMGAbilitySystemComponent* ASC = WeakASC.Get())
				{
					ASC->TryActivatePendingCombo(CachedComboTag);
				}
			})
		);
	}
}
