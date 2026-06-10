// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/MGAbilitySet.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MGGameplayAbility.h"
#include "MG.h"

void FMGAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FMGAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FMGAbilitySet_GrantedHandles::TakeFromAbilitySystem(UMGAbilitySystemComponent* ASC)
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}

void UMGAbilitySet::GiveToAbilitySystem(UMGAbilitySystemComponent* ASC, FMGAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 i = 0; i < GrantedGameplayAbilities.Num(); ++i)
	{
		const FMGAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[i];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogMG, Error, TEXT("UMGAbilitySet::GiveToAbilitySystem — GrantedGameplayAbilities[%d] is invalid."), i);
			continue;
		}

		UMGGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMGGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(Handle);
		}
	}

	for (int32 i = 0; i < GrantedGameplayEffects.Num(); ++i)
	{
		const FMGAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[i];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogMG, Error, TEXT("UMGAbilitySet::GiveToAbilitySystem — GrantedGameplayEffects[%d] is invalid."), i);
			continue;
		}

		const UGameplayEffect* EffectCDO = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectToSelf(EffectCDO, EffectToGrant.EffectLevel, ASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(Handle);
		}
	}
}
