// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MGAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MGHealthSet.generated.h"

UCLASS(BlueprintType)
class MG_API UMGHealthSet : public UMGAttributeSet
{
	GENERATED_BODY()

public:
	UMGHealthSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	ATTRIBUTE_ACCESSORS(UMGHealthSet, Health)
	ATTRIBUTE_ACCESSORS(UMGHealthSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UMGHealthSet, Shield)

private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "MG|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "MG|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Absorbs damage before Health. Not clamped to MaxHealth.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "MG|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldValue);
};
