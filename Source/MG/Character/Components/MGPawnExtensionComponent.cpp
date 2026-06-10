// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/Components/MGPawnExtensionComponent.h"
#include "MGGameplayTags.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/MGPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Net/UnrealNetwork.h"
#include "MG.h"
#include "GameFramework/Character.h"

const FName UMGPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UMGPawnExtensionComponent::UMGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick          = false;
	SetIsReplicatedByDefault(true);
}

void UMGPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMGPawnExtensionComponent, PawnData);
}

void UMGPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	RegisterInitStateFeature();
}

void UMGPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for state changes on every feature of this actor.
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	ensure(TryToChangeInitState(MGGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UMGPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

// -----------------------------------------------------------------------------

bool UMGPawnExtensionComponent::CanChangeInitState(
	UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);
	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == MGGameplayTags::InitState_Spawned)
	{
		return Pawn != nullptr;
	}
	if (CurrentState == MGGameplayTags::InitState_Spawned && DesiredState == MGGameplayTags::InitState_DataAvailable)
	{
		return PawnData != nullptr;
	}
	if (CurrentState == MGGameplayTags::InitState_DataAvailable && DesiredState == MGGameplayTags::InitState_DataInitialized)
	{
		// Gate on ASC being wired up AND all sibling features reaching DataAvailable.
		return AbilitySystemComponent != nullptr &&
		       Manager->HaveAllFeaturesReachedInitState(Pawn, MGGameplayTags::InitState_DataAvailable);
	}
	if (CurrentState == MGGameplayTags::InitState_DataInitialized && DesiredState == MGGameplayTags::InitState_GameplayReady)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, MGGameplayTags::InitState_DataInitialized);
	}
	return false;
}

void UMGPawnExtensionComponent::HandleChangeInitState(
	UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	if (DesiredState == MGGameplayTags::InitState_GameplayReady)
	{
		// Apply ABP and default linked layers from PawnData.
		if (PawnData && PawnData->AnimClass)
		{
			if (ACharacter* Character = GetPawn<ACharacter>())
			{
				USkeletalMeshComponent* Mesh = Character->GetMesh();
				Mesh->SetAnimInstanceClass(PawnData->AnimClass);

				if (PawnData->DefaultLocomotionLayer)
				{
					Mesh->LinkAnimClassLayers(PawnData->DefaultLocomotionLayer);
				}
				if (PawnData->DefaultCombatLayer)
				{
					Mesh->LinkAnimClassLayers(PawnData->DefaultCombatLayer);
				}
			}
		}

		// Both PawnData and ASC are guaranteed at this point.
		// Broadcast so features (e.g. AMGPlayerCharacter) can grant abilities and bind input.
		OnAbilitySystemInitialized.Broadcast();
	}
}

void UMGPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		CheckDefaultInitialization();
	}
}

void UMGPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();
	ContinueInitStateChain({
		MGGameplayTags::InitState_Spawned,
		MGGameplayTags::InitState_DataAvailable,
		MGGameplayTags::InitState_DataInitialized,
		MGGameplayTags::InitState_GameplayReady,
	});
}

// -----------------------------------------------------------------------------

void UMGPawnExtensionComponent::SetPawnData(const UMGPawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn || Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	if (PawnData)
	{
		UE_LOG(LogMG, Error, TEXT("[%s] SetPawnData called but PawnData is already set."), *GetNameSafe(Pawn));
		return;
	}

	PawnData = InPawnData;
	Pawn->ForceNetUpdate();
	CheckDefaultInitialization();
}

void UMGPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

// -----------------------------------------------------------------------------

void UMGPawnExtensionComponent::InitializeAbilitySystem(UMGAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC && InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		return;
	}
	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	// OnAbilitySystemInitialized is broadcast in HandleChangeInitState → GameplayReady,
	// where both ASC and PawnData are guaranteed to be valid.
	CheckDefaultInitialization();
}

void UMGPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->RemoveAllGameplayCues();
	}

	AbilitySystemComponent = nullptr;
	OnAbilitySystemUninitialized.Broadcast();
}

void UMGPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void UMGPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}
