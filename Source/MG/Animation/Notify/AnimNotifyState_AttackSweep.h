// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/Components/MGCombatTypes.h"
#include "AnimNotifyState_AttackSweep.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Waepon Sweep")
class MG_API UAnimNotifyState_AttackSweep : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UAnimNotifyState_AttackSweep();

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
    // 기획자가 에디터에서 세팅할 파라미터들 [cite: 8]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep Settings")
    FName BaseSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep Settings")
    FName TipSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep Settings")
    float SweepRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
    float DamageMultiplier;

    // 이 스윕에 맞았을 때 피격자가 재생할 피격 반응 강도 (약피격 / 강피격)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
    EMGHitReactionType HitReactionType;
};
