// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MGCharacter.generated.h"

class UMGAbilitySystemComponent;
class UMGPawnExtensionComponent;
class UMGCombatComponent;
class UMGPawnData;

/**
 * Base character class for all MG pawns (players and AI).
 *
 * - Owns UMGPawnExtensionComponent which drives the init-state chain.
 * - ASC is NOT owned here; it lives on MGPlayerState (players) or the
 *   enemy pawn itself (AI). GetAbilitySystemComponent() routes through
 *   PawnExtensionComponent so callers never need to care which path is used.
 * - Camera and input setup are left to subclasses.
 */
UCLASS(abstract)
class MG_API AMGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMGCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	UFUNCTION(BlueprintPure, Category="MG|Ability")
	UMGAbilitySystemComponent* GetMGAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category="MG|Pawn")
	UMGPawnExtensionComponent* GetPawnExtensionComponent() const { return PawnExtensionComponent; }

	UFUNCTION(BlueprintPure, Category="MG|Combat")
	UMGCombatComponent* GetCombatComponent() const { return CombatComponent; }

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Optional default PawnData used when the Experience system is not running.
	// Set in the Blueprint class defaults; pushed to PawnExtensionComponent on BeginPlay (server).
	UPROPERTY(EditDefaultsOnly, Category="MG|Pawn")
	TObjectPtr<const UMGPawnData> DefaultPawnData;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MG|Components", meta=(AllowPrivateAccess))
	TObjectPtr<UMGPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MG|Components", meta=(AllowPrivateAccess))
	TObjectPtr<UMGCombatComponent> CombatComponent;
};
