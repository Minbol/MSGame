// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/Combo/MGComboTableDataAsset.h"
#include "AbilitySystem/ChainSkill/MGChainSkillTableDataAsset.h"
#include "AbilitySystemGlobals.h"
#include "MGGameplayTags.h"

UMGAbilitySystemComponent::UMGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMGAbilitySystemComponent* UMGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool bLookForComponent)
{
	return Cast<UMGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, bLookForComponent));
}

void UMGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	// 연계기 윈도우 조회를 위해 홀드 중인 입력을 항상 추적합니다.
	CurrentlyHeldInputTags.Add(InputTag);

	// 공격 어빌리티가 활성화된 상태에서 공격 입력이 들어오면 콤보 버퍼에 저장합니다.
	static const FGameplayTag AttackAbilityParent = FGameplayTag::RequestGameplayTag(FName("MG.Ability.Attack"));
	static const FGameplayTag AttackInputParent   = FGameplayTag::RequestGameplayTag(FName("MG.Input.Attack"));

	if (InputTag.MatchesTag(AttackInputParent) && HasMatchingGameplayTag(AttackAbilityParent))
	{
		PendingComboInputTag = InputTag;
		return;
	}

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputPressedSpecHandles.AddUnique(Spec.Handle);
			InputHeldSpecHandles.AddUnique(Spec.Handle);
		}
	}
}

void UMGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	CurrentlyHeldInputTags.Remove(InputTag);

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputReleasedSpecHandles.AddUnique(Spec.Handle);
			InputHeldSpecHandles.Remove(Spec.Handle);
		}
	}
}

void UMGAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (bGamePaused)
	{
		ClearAbilityInput();
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle))
		{
			if (Spec->Ability)
			{
				Spec->InputPressed = true;

				if (Spec->IsActive())
				{
					AbilitySpecInputPressed(*Spec);
				}
				else
				{
					TryActivateAbility(Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle))
		{
			if (Spec->Ability)
			{
				Spec->InputPressed = false;

				if (Spec->IsActive())
				{
					AbilitySpecInputReleased(*Spec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UMGAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

bool UMGAbilitySystemComponent::TryActivatePendingCombo(const FGameplayTag& CurrentAbilityTag)
{
	if (!PendingComboInputTag.IsValid() || !ComboTableAsset)
	{
		ClearPendingComboInput();
		return false;
	}

	const FGameplayTag NextTag = ComboTableAsset->ResolveNextAbility(CurrentAbilityTag, PendingComboInputTag);
	ClearPendingComboInput();

	if (!NextTag.IsValid())
	{
		return false;
	}

	FGameplayTagContainer NextTagContainer;
	NextTagContainer.AddTag(NextTag);
	return TryActivateAbilitiesByTag(NextTagContainer);
}

void UMGAbilitySystemComponent::ClearPendingComboInput()
{
	PendingComboInputTag = FGameplayTag::EmptyTag;
}

bool UMGAbilitySystemComponent::HasChainSkillInput(const FGameplayTag& CurrentAbilityTag) const
{
	if (!ChainSkillTableAsset || !CurrentAbilityTag.IsValid())
	{
		return false;
	}
	return ChainSkillTableAsset->ResolveNextAbility(CurrentAbilityTag, CurrentlyHeldInputTags).IsValid();
}

bool UMGAbilitySystemComponent::TryActivateChainSkill(const FGameplayTag& CurrentAbilityTag)
{
	if (!ChainSkillTableAsset || !CurrentAbilityTag.IsValid())
	{
		return false;
	}

	const FGameplayTag NextTag = ChainSkillTableAsset->ResolveNextAbility(CurrentAbilityTag, CurrentlyHeldInputTags);
	if (!NextTag.IsValid())
	{
		return false;
	}

	FGameplayTagContainer NextTagContainer;
	NextTagContainer.AddTag(NextTag);
	return TryActivateAbilitiesByTag(NextTagContainer);
}
