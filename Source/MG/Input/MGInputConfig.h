// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MGInputConfig.generated.h"

class UInputAction;

/** One Tag ↔ InputAction pair used in UMGInputConfig. */
USTRUCT(BlueprintType)
struct FMGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(Categories="MG.Input"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
};

/**
 * DataAsset that maps GameplayTags to InputActions.
 *
 * NativeInputActions  — bound directly to character functions (Move, Look, Sprint).
 * AbilityInputActions — routed through the ASC via AbilityInputTagPressed/Released.
 *
 * Create one DA_InputConfig_Player asset and reference it from UMGPawnData.
 */
UCLASS(BlueprintType, Const, Meta=(DisplayName="MG Input Config"))
class MG_API UMGInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	// Bound directly to AMGPlayerCharacter functions (no GAS).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Native")
	TArray<FMGInputAction> NativeInputActions;

	// Bound to GAS via tag-based ASC callbacks.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Ability")
	TArray<FMGInputAction> AbilityInputActions;
};
