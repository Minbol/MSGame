// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MGCombatSet.generated.h"

UCLASS(BlueprintType)
class MG_API UMGCombatSet : public UMGAttributeSet
{
	GENERATED_BODY()

public:
	UMGCombatSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	ATTRIBUTE_ACCESSORS(UMGCombatSet, AttackPower)
	ATTRIBUTE_ACCESSORS(UMGCombatSet, Defense)
	// CritRate: 0.0 ~ 1.0
	ATTRIBUTE_ACCESSORS(UMGCombatSet, CritRate)

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "MG|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "MG|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritRate, Category = "MG|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CritRate;

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CritRate(const FGameplayAttributeData& OldValue);
};
