// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MGInputConfig.h"
#include "MG.h"

const UInputAction* UMGInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMGInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogMG, Warning, TEXT("UMGInputConfig: no native action for tag [%s] on %s"),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

const UInputAction* UMGInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMGInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogMG, Warning, TEXT("UMGInputConfig: no ability action for tag [%s] on %s"),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
