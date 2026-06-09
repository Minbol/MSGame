// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "MGAbility_Dodge.generated.h"

/**
 * Dodge roll. Inherits montage playback from UMGAbility_MontageBase.
 *
 * OnBeforeMontagePlayed launches the character in the last movement input direction.
 * While active the ability owns MG.Ability.Dodge (re-activation guard) and MG.Status.Invincible
 * (i-frame hook for the damage system). Both are removed automatically when the ability ends.
 */
UCLASS()
class MG_API UMGAbility_Dodge : public UMGAbility_MontageBase
{
	GENERATED_BODY()

public:
	UMGAbility_Dodge(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Horizontal impulse applied at dodge start (cm/s).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Dodge")
	float DodgeImpulse = 900.f;

protected:
	virtual void OnBeforeMontagePlayed() override;
};
