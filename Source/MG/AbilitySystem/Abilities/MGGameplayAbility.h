// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MGGameplayAbility.generated.h"

class UMGAbilitySystemComponent;
class AMGCharacter;

/**
 * Base class for all MG gameplay abilities.
 * Provides typed helpers for ASC, Controller, and Character access.
 */
UCLASS(Abstract, BlueprintType)
class MG_API UMGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMGGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "MG|Ability")
	UMGAbilitySystemComponent* GetMGAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "MG|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "MG|Ability")
	AMGCharacter* GetMGCharacterFromActorInfo() const;

protected:
	// 엔진 기본 구현은 빈 함수라 WaitInputPress/WaitInputRelease 태스크가 절대 발화하지 않습니다.
	// ASC 로 InvokeReplicatedEvent 를 직접 쏴줘야 태스크의 델리게이트가 실행됩니다.
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
};
