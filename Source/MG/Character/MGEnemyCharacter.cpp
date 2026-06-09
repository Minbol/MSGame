// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MGEnemyCharacter.h"
#include "Character/Components/MGPawnExtensionComponent.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MGHealthSet.h"
#include "AbilitySystem/Attributes/MGCombatSet.h"

AMGEnemyCharacter::AMGEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UMGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// AttributeSets are owned by the pawn — registered with the ASC via creation.
	HealthSet = CreateDefaultSubobject<UMGHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UMGCombatSet>(TEXT("CombatSet"));
}

void AMGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay(); // pushes DefaultPawnData if set

	// AI owns ASC directly — actor and avatar are both this pawn.
	GetPawnExtensionComponent()->InitializeAbilitySystem(AbilitySystemComponent, this);
}
