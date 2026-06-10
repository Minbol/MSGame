// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MGPlayerState.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MGHealthSet.h"
#include "AbilitySystem/Attributes/MGStaminaSet.h"
#include "AbilitySystem/Attributes/MGCombatSet.h"

AMGPlayerState::AMGPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// GAS needs fast updates to keep attribute changes snappy on clients.
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMGAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode: server replicates GEs and tags to all; attribute changes only to owner.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AttributeSets are subobjects of the PlayerState; registering them with the ASC
	// happens automatically when they are created as subobjects of the ASC's owner.
	HealthSet  = ObjectInitializer.CreateDefaultSubobject<UMGHealthSet>(this, TEXT("HealthSet"));
	StaminaSet = ObjectInitializer.CreateDefaultSubobject<UMGStaminaSet>(this, TEXT("StaminaSet"));
	CombatSet  = ObjectInitializer.CreateDefaultSubobject<UMGCombatSet>(this, TEXT("CombatSet"));
}

UAbilitySystemComponent* AMGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
