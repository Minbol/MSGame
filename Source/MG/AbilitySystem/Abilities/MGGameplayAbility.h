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
};
