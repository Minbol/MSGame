// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "MGPawnExtensionComponent.generated.h"

class UMGAbilitySystemComponent;
class UMGPawnData;

/**
 * Central orchestrator for pawn initialization.
 *
 * Add to every AMGCharacter. Drives the init-state chain:
 *   Spawned → DataAvailable → DataInitialized → GameplayReady
 *
 * PawnData is replicated so both server and client advance the chain
 * independently. Characters call InitializeAbilitySystem() once the
 * appropriate ASC becomes available (PlayerState for players, self for AI).
 */
UCLASS(ClassGroup=MG, meta=(BlueprintSpawnableComponent))
class MG_API UMGPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UMGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;

	static UMGPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UMGPawnExtensionComponent>() : nullptr;
	}

	//~ IGameFrameworkInitStateInterface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface

	const UMGPawnData* GetPawnData() const { return PawnData; }

	// Server-only. Called by the Experience system (or character itself) to supply PawnData.
	void SetPawnData(const UMGPawnData* InPawnData);

	UMGAbilitySystemComponent* GetMGAbilitySystemComponent() const { return AbilitySystemComponent; }

	// Called by the character once the right ASC owner is known.
	// InOwnerActor = PlayerState for players, the pawn itself for AI.
	void InitializeAbilitySystem(UMGAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	// Forwarded from AMGCharacter so the state machine can react.
	void HandleControllerChanged();
	void HandlePlayerStateReplicated();

	DECLARE_MULTICAST_DELEGATE(FMGAbilitySystemEvent);
	FMGAbilitySystemEvent OnAbilitySystemInitialized;
	FMGAbilitySystemEvent OnAbilitySystemUninitialized;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_PawnData)
	TObjectPtr<const UMGPawnData> PawnData;

	UFUNCTION()
	void OnRep_PawnData();

	UPROPERTY()
	TObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;
};
