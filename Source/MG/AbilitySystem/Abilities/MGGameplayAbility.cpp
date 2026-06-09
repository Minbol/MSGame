// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGGameplayAbility.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "Character/MGCharacter.h"

UMGGameplayAbility::UMGGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Abilities are not instanced by default; override in subclasses if state is needed.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UMGAbilitySystemComponent* UMGGameplayAbility::GetMGAbilitySystemComponent() const
{
	return Cast<UMGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

AController* UMGGameplayAbility::GetControllerFromActorInfo() const
{
	if (!CurrentActorInfo)
	{
		return nullptr;
	}

	if (AController* PC = CurrentActorInfo->PlayerController.Get())
	{
		return PC;
	}

	// Fallback for AI controllers.
	if (APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get()))
	{
		return Pawn->GetController();
	}

	return nullptr;
}

AMGCharacter* UMGGameplayAbility::GetMGCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AMGCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}
