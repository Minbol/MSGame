// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "MGAbilitySet.generated.h"

class UMGGameplayAbility;
class UMGAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FMGAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMGGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Input tag bound when the ability is granted (wired up in step 4).
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "MG.Input"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FMGAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.f;
};

// Holds handles returned from GiveToAbilitySystem so they can be revoked later.
USTRUCT(BlueprintType)
struct FMGAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	// Removes everything that was granted and clears the handle arrays.
	void TakeFromAbilitySystem(UMGAbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

/**
 * Groups a set of abilities and effects that can be granted to / revoked from an ASC as a unit.
 * Referenced from UMGPawnData; not a PrimaryDataAsset since it's always loaded via hard reference.
 */
UCLASS(BlueprintType)
class MG_API UMGAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	// Grants all abilities and effects to the given ASC.
	// OutGrantedHandles receives handles that can later be passed to TakeFromAbilitySystem.
	void GiveToAbilitySystem(UMGAbilitySystemComponent* ASC, FMGAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FMGAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<FMGAbilitySet_GameplayEffect> GrantedGameplayEffects;
};
