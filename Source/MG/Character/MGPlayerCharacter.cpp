// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MGPlayerCharacter.h"
#include "Character/Components/MGPawnExtensionComponent.h"
#include "Player/MGPlayerState.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "AbilitySystem/MGPawnData.h"
#include "AbilitySystem/MGAbilitySet.h"
#include "Input/MGInputComponent.h"
#include "Input/MGInputConfig.h"
#include "MGGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AMGPlayerCharacter::AMGPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength         = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMGPlayerCharacter::BeginPlay()
{
	// Blueprint class defaults can override bCanEverTick / bStartWithTickEnabled.
	// We enforce ticking here since ProcessAbilityInput depends on it.
	SetActorTickEnabled(true);

	// Bind BEFORE Super::BeginPlay() — component BeginPlay (called inside Super) may
	// advance the init-state chain all the way to GameplayReady and broadcast immediately
	// if PawnData is already set (e.g. DefaultPawnData in BP class defaults).
	UMGPawnExtensionComponent* PawnExt = GetPawnExtensionComponent();
	PawnExt->OnAbilitySystemInitialized.AddUObject(this, &ThisClass::OnAbilitySystemInitialized);
	PawnExt->OnAbilitySystemUninitialized.AddUObject(this, &ThisClass::OnAbilitySystemUninitialized);

	Super::BeginPlay();
}

void AMGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializePlayerASC();
}

void AMGPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializePlayerASC();
}

void AMGPlayerCharacter::InitializePlayerASC()
{
	AMGPlayerState* PS = GetPlayerState<AMGPlayerState>();
	if (!PS)
	{
		return;
	}

	// InitializeAbilitySystem is idempotent — safe to call from both server and client paths.
	GetPawnExtensionComponent()->InitializeAbilitySystem(PS->GetMGAbilitySystemComponent(), PS);
}

void AMGPlayerCharacter::OnAbilitySystemInitialized()
{
	// Grant abilities from PawnData.
	if (const UMGPawnData* PawnData = GetPawnExtensionComponent()->GetPawnData())
	{
		if (PawnData->AbilitySet)
		{
			UMGAbilitySystemComponent* ASC = GetPawnExtensionComponent()->GetMGAbilitySystemComponent();
			PawnData->AbilitySet->GiveToAbilitySystem(ASC, &AbilitySetHandles, this);
		}
	}

	// Fallback: bind input if SetupPlayerInputComponent ran before PawnData was ready.
	BindInputActions();
}

void AMGPlayerCharacter::OnAbilitySystemUninitialized()
{
	// Revoke abilities granted from PawnData.
	if (UMGAbilitySystemComponent* ASC = GetPawnExtensionComponent()->GetMGAbilitySystemComponent())
	{
		AbilitySetHandles.TakeFromAbilitySystem(ASC);
	}

	// Remove ability input bindings so they aren't fired against a stale ASC.
	if (UMGInputComponent* MGIC = Cast<UMGInputComponent>(InputComponent))
	{
		for (uint32 Handle : AbilityInputBindingHandles)
		{
			MGIC->RemoveBindingByHandle(Handle);
		}
	}
	AbilityInputBindingHandles.Reset();
}

void AMGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add the default mapping context via the local player's EnhancedInput subsystem.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	// PawnData may not have arrived yet (client replication or DefaultPawnData not set).
	// BindInputActions() is also called from OnAbilitySystemInitialized as a fallback.
	BindInputActions();
}

void AMGPlayerCharacter::BindInputActions()
{
	// Need InputComponent (assigned by engine in SetupPlayerInputComponent).
	UMGInputComponent* MGIC = Cast<UMGInputComponent>(InputComponent);
	if (!MGIC)
	{
		return;
	}

	// Guard against double-binding: AbilityInputBindingHandles is empty only before first bind.
	if (!AbilityInputBindingHandles.IsEmpty())
	{
		return;
	}

	const UMGPawnData* PawnData = GetPawnExtensionComponent()->GetPawnData();
	if (!PawnData)
	{
		return;
	}

	const UMGInputConfig* InputConfig = PawnData->InputConfig;
	if (!InputConfig)
	{
		return;
	}

	// Native actions.
	MGIC->BindNativeAction(InputConfig, MGGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move);

	MGIC->BindNativeAction(InputConfig, MGGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	// Ability actions — all bound at once; tag is passed to each callback.
	MGIC->BindAbilityActions(InputConfig, this,
		&ThisClass::Input_AbilityInputTagPressed,
		&ThisClass::Input_AbilityInputTagReleased,
		AbilityInputBindingHandles);
}

void AMGPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocallyControlled())
	{
		if (UMGAbilitySystemComponent* ASC = GetPawnExtensionComponent()->GetMGAbilitySystemComponent())
		{
			ASC->ProcessAbilityInput(DeltaSeconds, false);
		}
	}
}

void AMGPlayerCharacter::Input_Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector2D MoveVec = Value.Get<FVector2D>();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, MoveVec.Y);
		AddMovementInput(RightDir,   MoveVec.X);
	}
}

void AMGPlayerCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	AddControllerYawInput(LookVec.X);
	AddControllerPitchInput(-LookVec.Y);
}

void AMGPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UMGAbilitySystemComponent* ASC = GetPawnExtensionComponent()->GetMGAbilitySystemComponent())
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void AMGPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UMGAbilitySystemComponent* ASC = GetPawnExtensionComponent()->GetMGAbilitySystemComponent())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}
