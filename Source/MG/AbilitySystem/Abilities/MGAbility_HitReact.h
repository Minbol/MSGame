// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGGameplayAbility.h"
#include "MGAbility_HitReact.generated.h"

/**
 * 피격 반응 어빌리티.
 *
 * MGCombatComponent 의 무기 스윕이 명중하면 대상 ASC 에 MG.Event.Hit 이벤트가 전송되고,
 * AbilityTriggers 설정에 의해 이 어빌리티가 자동으로 활성화됩니다 (입력 불필요).
 *
 * 이벤트 페이로드의 InstigatorTags 에 실린 MG.Combat.HitReaction.Weak / Strong 태그를 보고
 * WeakHitMontage 또는 StrongHitMontage 중 하나를 재생합니다.
 */
UCLASS()
class MG_API UMGAbility_HitReact : public UMGGameplayAbility
{
	GENERATED_BODY()

public:
	UMGAbility_HitReact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 약피격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|HitReact")
	TObjectPtr<UAnimMontage> WeakHitMontage;

	// 강피격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|HitReact")
	TObjectPtr<UAnimMontage> StrongHitMontage;

private:
	UFUNCTION()
	void OnMontageEnded();

	UFUNCTION()
	void OnMontageCancelled();
};
