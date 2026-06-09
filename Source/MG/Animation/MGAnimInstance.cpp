// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animation/MGAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMGAnimInstance::UMGAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* Owner = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			AbilitySystemComponent = Cast<UMGAbilitySystemComponent>(ASC);
			GameplayTagPropertyMap.Initialize(this, ASC);
		}
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

	const ACharacter* Character = Cast<ACharacter>(Pawn);
	const UCharacterMovementComponent* CMC = Character ? Character->GetCharacterMovement() : nullptr;

	if (CMC)
	{
		const FVector Velocity = CMC->Velocity;
		GroundSpeed   = Velocity.Size2D();
		bIsInAir      = CMC->IsFalling();
		bIsCrouching  = CMC->IsCrouching();
		bIsSprinting  = !bIsInAir && (GroundSpeed > CMC->MaxWalkSpeed * 0.8f);

		if (GroundSpeed > UE_KINDA_SMALL_NUMBER)
		{
			const FVector WorldDir = Velocity.GetSafeNormal2D();
			const FVector LocalDir = Pawn->GetActorTransform().InverseTransformVectorNoScale(WorldDir);
			// LocalDir.Y = right component, LocalDir.X = forward component
			MoveDirection = FVector2D(LocalDir.Y, LocalDir.X);
		}
		else
		{
			MoveDirection = FVector2D::ZeroVector;
		}
	}

	if (const AController* Controller = Pawn->GetController())
	{
		const FRotator Delta = (Controller->GetControlRotation() - Pawn->GetActorRotation()).GetNormalized();
		AimPitch = FMath::ClampAngle(Delta.Pitch, -90.f, 90.f);
		AimYaw   = FMath::ClampAngle(Delta.Yaw,  -180.f, 180.f);
	}
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
