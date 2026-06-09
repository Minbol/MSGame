// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MGAbilitySystemComponent.generated.h"

UCLASS()
class MG_API UMGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Returns the typed ASC from any Actor that implements IAbilitySystemInterface.
	static UMGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool bLookForComponent = false);

	// Called by UMGInputComponent bindings each frame (Triggered / Completed events).
	// The tag is matched against DynamicAbilityTags on each spec (set at grant time).
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// Call once per tick (e.g. from AMGPlayerCharacter::Tick) to flush pressed/released queues.
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Clears all pending input queues (called on ability system uninit or game pause).
	void ClearAbilityInput();

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
