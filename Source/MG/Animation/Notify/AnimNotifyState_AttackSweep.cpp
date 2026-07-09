// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNotifyState_AttackSweep.h"
#include "Character/Components/MGCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"

UAnimNotifyState_AttackSweep::UAnimNotifyState_AttackSweep()
{
    // 기본값 세팅
    BaseSocketName = TEXT("Weapon_Base");
    TipSocketName = TEXT("Weapon_Tip");
    SweepRadius = 15.0f;
    DamageMultiplier = 1.0f;
    HitReactionType = EMGHitReactionType::Weak;
}

void UAnimNotifyState_AttackSweep::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
    
    if (MeshComp && MeshComp->GetOwner())
    {
        // 액터에 붙어있는 전투 컴포넌트를 찾아 Sweep 시작을 알림
        if (UMGCombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UMGCombatComponent>())
        {
            CombatComp->BeginWeaponSweep(BaseSocketName, TipSocketName, SweepRadius, DamageMultiplier, HitReactionType);
        }
    }
}

void UAnimNotifyState_AttackSweep::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        // 매 프레임마다 궤적 추적 연산 지시
        if (UMGCombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UMGCombatComponent>())
        {
            CombatComp->TickWeaponSweep();
        }
    }
}

void UAnimNotifyState_AttackSweep::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (MeshComp && MeshComp->GetOwner())
    {
        // 타격 기록(Hit History) 초기화 및 Sweep 종료
        if (UMGCombatComponent* CombatComp = MeshComp->GetOwner()->FindComponentByClass<UMGCombatComponent>())
        {
            CombatComp->EndWeaponSweep();
        }
    }
}
