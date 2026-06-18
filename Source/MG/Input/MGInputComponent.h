// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/MGInputConfig.h"
#include "MGInputComponent.generated.h"

/**
 * UEnhancedInputComponent subclass with two helpers:
 *
 *  BindNativeAction    — looks up a tag in NativeInputActions and binds it directly.
 *  BindAbilityActions  — binds every AbilityInputAction to a pair of (Pressed / Released)
 *                        callbacks, each receiving the FGameplayTag so the ASC can route it.
 */
UCLASS(Config=Input)
class MG_API UMGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Bind one native action (Move, Look…) by tag.
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UMGInputConfig* Config, const FGameplayTag& InputTag,
	                      ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func,
	                      bool bLogIfNotFound = true);

	// Bind every ability action, passing InputTag to each callback.
	// Callbacks must have signature: void(FGameplayTag)
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMGInputConfig* Config,
	                        UserClass* Object,
	                        PressedFuncType  PressedFunc,
	                        ReleasedFuncType ReleasedFunc,
	                        TArray<uint32>& OutBindingHandles);
};

// ── template implementations ──────────────────────────────────────────────────

template<class UserClass, typename FuncType>
void UMGInputComponent::BindNativeAction(const UMGInputConfig* Config, const FGameplayTag& InputTag,
                                          ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func,
                                          bool bLogIfNotFound)
{
	check(Config);
	if (const UInputAction* IA = Config->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMGInputComponent::BindAbilityActions(const UMGInputConfig* Config,
                                            UserClass* Object,
                                            PressedFuncType  PressedFunc,
                                            ReleasedFuncType ReleasedFunc,
                                            TArray<uint32>& OutBindingHandles)
{
	check(Config);
	for (const FMGInputAction& Action : Config->AbilityInputActions)
	{
		if (!Action.InputAction || !Action.InputTag.IsValid())
		{
			continue;
		}

		if (PressedFunc)
		{
			FEnhancedInputActionEventBinding& B =
				BindAction(Action.InputAction, Action.PressTriggerEvent, Object, PressedFunc, Action.InputTag);
			OutBindingHandles.Add(B.GetHandle());
		}
		if (ReleasedFunc)
		{
			FEnhancedInputActionEventBinding& B =
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			OutBindingHandles.Add(B.GetHandle());
		}
	}
}
