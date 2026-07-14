// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MGCharacter.h"
#include "AbilitySystem/MGAbilitySet.h"
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

	// 몬스터 최대 체력 — 몬스터 BP(예: BP_Monster)에서 설정합니다. BeginPlay 에서 HealthSet 에 반영됩니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MG|Health")
	float MaxHealth = 100.f;

protected:
	virtual void BeginPlay() override;

private:
	// Called when PawnExtensionComponent broadcasts OnAbilitySystemInitialized — grants PawnData->AbilitySet.
	void OnAbilitySystemInitialized();

	UPROPERTY(VisibleAnywhere, Category="MG|Ability")
	TObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMGHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UMGCombatSet> CombatSet;

	// Handles returned when the AbilitySet is granted — used to revoke on uninit.
	FMGAbilitySet_GrantedHandles AbilitySetHandles;
};
