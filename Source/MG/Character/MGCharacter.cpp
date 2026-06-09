// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MGCharacter.h"
#include "Character/Components/MGPawnExtensionComponent.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/MGPawnData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMGCharacter::AMGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick         = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	GetCharacterMovement()->bOrientRotationToMovement  = true;
	GetCharacterMovement()->RotationRate               = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity              = 500.f;
	GetCharacterMovement()->AirControl                 = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed               = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed         = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	PawnExtensionComponent = CreateDefaultSubobject<UMGPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

UAbilitySystemComponent* AMGCharacter::GetAbilitySystemComponent() const
{
	return GetMGAbilitySystemComponent();
}

UMGAbilitySystemComponent* AMGCharacter::GetMGAbilitySystemComponent() const
{
	return PawnExtensionComponent ? PawnExtensionComponent->GetMGAbilitySystemComponent() : nullptr;
}

void AMGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// When no Experience system is running, push the BP-authored default PawnData now.
	if (HasAuthority() && DefaultPawnData && !PawnExtensionComponent->GetPawnData())
	{
		PawnExtensionComponent->SetPawnData(DefaultPawnData);
	}
}

void AMGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnExtensionComponent->HandleControllerChanged();
}

void AMGCharacter::UnPossessed()
{
	Super::UnPossessed();
	PawnExtensionComponent->UninitializeAbilitySystem();
	PawnExtensionComponent->HandleControllerChanged();
}

void AMGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	PawnExtensionComponent->HandlePlayerStateReplicated();
}

void AMGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
