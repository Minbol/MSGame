// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MGCharacter.h"
#include "MGEnemyCharacter.generated.h"

class UMGAbilitySystemComponent;
class UMGHealthSet;
class UMGCombatSet;

/**
 * AI / enemy character.
 *
 * Owns its AbilitySystemComponent directly on the pawn (no PlayerState).
 * Replication mode is Minimal since enemies don't use client prediction.
 * AttributeSets are owned here and registered with the ASC in the constructor.
 */
UCLASS()
class MG_API AMGEnemyCharacter : public AMGCharacter
{
	GENERATED_BODY()

public:
	AMGEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="MG|Ability")
	TObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMGHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UMGCombatSet> CombatSet;
};
