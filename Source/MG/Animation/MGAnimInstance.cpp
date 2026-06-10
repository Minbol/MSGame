// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animation/MGAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MGGameplayTags.h"

UMGAnimInstance::UMGAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* Owner = GetOwningActor())
	{
		AbilitySystemComponent = Cast<UMGAbilitySystemComponent>(
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner));
	}
}

void UMGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn)
	{
		return;
	}

	// ── Locomotion ─────────────────────────────────────────────────────────────

	const ACharacter* Character = Cast<ACharacter>(Pawn);
	const UCharacterMovementComponent* CMC = Character ? Character->GetCharacterMovement() : nullptr;

	if (CMC)
	{
		const FVector Velocity = CMC->Velocity;
		GroundSpeed  = Velocity.Size2D();
		VelocityZ    = Velocity.Z;
		bIsInAir     = CMC->IsFalling();
		bIsCrouching = CMC->IsCrouching();
		bIsSprinting = !bIsInAir && (GroundSpeed > CMC->MaxWalkSpeed * 0.8f);
		bShouldMove  = GroundSpeed > UE_KINDA_SMALL_NUMBER && CMC->GetCurrentAcceleration().SizeSquared() > 0.f;

		if (GroundSpeed > UE_KINDA_SMALL_NUMBER)
		{
			const FVector LocalDir = Pawn->GetActorTransform().InverseTransformVectorNoScale(
				Velocity.GetSafeNormal2D());
			MoveDirection = FVector2D(LocalDir.Y, LocalDir.X);
			Direction = CalculateDirection(Velocity, Pawn->GetActorRotation());
		}
		else
		{
			MoveDirection = FVector2D::ZeroVector;
			Direction = 0.f;
		}
	}

	if (const AController* Controller = Pawn->GetController())
	{
		const FRotator Delta = (Controller->GetControlRotation() - Pawn->GetActorRotation()).GetNormalized();
		AimPitch = FMath::ClampAngle(Delta.Pitch, -90.f,  90.f);
		AimYaw   = FMath::ClampAngle(Delta.Yaw,  -180.f, 180.f);
	}

	// ── Status / Ability tags ──────────────────────────────────────────────────

	const UMGAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	using namespace MGGameplayTags;

	bIsDead          = ASC->HasMatchingGameplayTag(StatusTag_Dead);
	bIsKnockback     = ASC->HasMatchingGameplayTag(StatusTag_Knockback);
	bIsStagger       = ASC->HasMatchingGameplayTag(StatusTag_Stagger);
	bIsInvincible    = ASC->HasMatchingGameplayTag(StatusTag_Invincible);
	bIsParrying      = ASC->HasMatchingGameplayTag(StatusTag_Parrying);
	bIsAttackingLight = ASC->HasMatchingGameplayTag(AbilityTag_Attack_Light);
	bIsAttackingHeavy = ASC->HasMatchingGameplayTag(AbilityTag_Attack_Heavy);
	bIsDodging       = ASC->HasMatchingGameplayTag(AbilityTag_Dodge);
	bIsJumping       = ASC->HasMatchingGameplayTag(AbilityTag_Jump);
}

void UMGAnimInstance::NativeUninitializeAnimation()
{
	AbilitySystemComponent.Reset();
	Super::NativeUninitializeAnimation();
}

UMGAbilitySystemComponent* UMGAnimInstance::GetMGAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}
