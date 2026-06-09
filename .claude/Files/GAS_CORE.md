# 2단계 GAS 코어 구조 설명

## 전체 그림

```
                   ┌─────────────────────────────────────┐
                   │           AMGPlayerState             │
                   │  ┌──────────────────────────────┐   │
                   │  │   UMGAbilitySystemComponent   │   │
                   │  └──────────────────────────────┘   │
                   │  ┌──────────┐ ┌───────────────────┐ │
                   │  │HealthSet │ │ StaminaSet        │ │
                   │  └──────────┘ └───────────────────┘ │
                   │  ┌──────────┐                        │
                   │  │CombatSet │                        │
                   │  └──────────┘                        │
                   └───────────────┬─────────────────────┘
                                   │ GetAbilitySystemComponent()
                   ┌───────────────▼─────────────────────┐
                   │              AMGCharacter            │
                   │         IAbilitySystemInterface      │
                   └─────────────────────────────────────┘
```

PlayerState가 ASC와 AttributeSet을 소유하고, Character는 IAbilitySystemInterface를
통해 PlayerState의 ASC를 가리킨다. 이 분리가 핵심이다.

---

## 왜 PlayerState가 ASC를 소유하는가

| 방식 | 장점 | 단점 |
|------|------|------|
| **Pawn 소유** | 단순 | 리스폰 시 Pawn 파괴 → ASC 리셋, 어빌리티·버프 소멸 |
| **PlayerState 소유** ✓ | 리스폰해도 ASC 유지, 버프/쿨다운 보존 | InitAbilityActorInfo 두 번 필요 |

AI는 리스폰 개념이 없으므로 Pawn에 직접 소유 (3단계에서 AMGEnemyCharacter에 구현).

---

## 파일별 역할

### 1. UMGAttributeSet (베이스)

```
AbilitySystem/MGAttributeSet.h
```

- `UAttributeSet`을 상속하는 추상 베이스
- `ATTRIBUTE_ACCESSORS` 매크로 한 곳에 정의
  - `GetXxxAttribute()` — FGameplayAttribute 반환 (GE에서 타겟 지정 시 사용)
  - `GetXxx() / SetXxx() / InitXxx()` — float 접근자

### 2. AttributeSet 3종

```
AbilitySystem/Attributes/
  MGHealthSet   — Health, MaxHealth, Shield
  MGStaminaSet  — Stamina, MaxStamina
  MGCombatSet   — AttackPower, Defense, CritRate(0~1)
```

**데이터 흐름 (attribute 변경 시):**

```
GameplayEffect 적용
    │
    ▼
PreAttributeBaseChange()   ← 베이스 값 클램핑 (영구 변경)
    │
    ▼
PreAttributeChange()       ← 현재 값 클램핑 (임시 modifier 포함)
    │
    ▼
(값 변경)
    │
    ▼
PostAttributeChange()      ← 사후 처리 (MaxHealth 감소 시 Health도 조정)
    │
    ▼
OnRep_Xxx()                ← 클라이언트에 복제 알림
```

**클램핑 규칙:**
- Health → `[0, MaxHealth]`
- MaxHealth → `[1, ∞)` (0이 되면 나누기 오류 방지)
- Shield → `[0, ∞)`
- Stamina → `[0, MaxStamina]`
- CritRate → `[0.0, 1.0]`

### 3. UMGAbilitySystemComponent

```
AbilitySystem/Components/MGAbilitySystemComponent.h
```

- `UAbilitySystemComponent` 직접 상속, 현재는 래퍼 수준
- `GetAbilitySystemComponentFromActor()` 정적 헬퍼 제공
- 향후 확장 포인트: 커스텀 어빌리티 태그 처리, 어빌리티 취소 로직, 디버그 출력

### 4. UMGGameplayAbility (어빌리티 베이스)

```
AbilitySystem/Abilities/MGGameplayAbility.h
```

- 모든 MG 어빌리티의 공통 베이스
- 타입 캐스팅 헬퍼 3종:
  ```cpp
  GetMGAbilitySystemComponent()   // UMGAbilitySystemComponent*
  GetControllerFromActorInfo()    // AI 컨트롤러 폴백 포함
  GetMGCharacterFromActorInfo()   // AMGCharacter*
  ```
- `InstancingPolicy = InstancedPerActor` 기본값
  - 어빌리티 인스턴스가 액터마다 하나 생성 → 상태(타이머, 참조) 저장 가능
  - 성능 중요 어빌리티는 `NonInstanced`로 오버라이드

### 5. UMGAbilitySet (어빌리티 묶음)

```
AbilitySystem/MGAbilitySet.h
```

어빌리티와 GameplayEffect를 하나의 DataAsset으로 묶어 ASC에 일괄 부여/회수.

```
DA_AbilitySet_Player
  GrantedGameplayAbilities:
    [0] Ability=GA_Jump,   Level=1, InputTag=MG.Input.Jump
    [1] Ability=GA_Dodge,  Level=1, InputTag=MG.Input.Dodge
    [2] Ability=GA_Attack, Level=1, InputTag=MG.Input.Attack.Light
  GrantedGameplayEffects:
    [0] Effect=GE_InitStats, Level=1   ← 시작 스탯 초기화
```

`GiveToAbilitySystem()` 호출 시 `FMGAbilitySet_GrantedHandles` 반환.
나중에 `TakeFromAbilitySystem()`으로 전부 회수 (무기 교체, 스킬 리셋 등).

### 6. UMGPawnData

```
AbilitySystem/MGPawnData.h
```

캐릭터 유형별 설정을 담는 PrimaryDataAsset.

```
DA_PawnData_Player
  AbilitySet = DA_AbilitySet_Player
  AnimClass  = ABP_MG_Main_C

DA_PawnData_Enemy_Grunt
  AbilitySet = DA_AbilitySet_Enemy_Grunt
  AnimClass  = ABP_MG_Enemy_Grunt_C
```

`UMGAssetManager::PawnDataType`으로 비동기 로드 가능.
`UMGGameData::DefaultPawnData`가 폴백 값.

### 7. AMGPlayerState

```
Player/MGPlayerState.h
```

- `NetUpdateFrequency = 100` — 기본 1Hz는 GAS에 너무 느림
- `ReplicationMode = Mixed`:
  - 서버 → 모든 클라이언트: GameplayCue, GameplayTag
  - 서버 → 소유 클라이언트만: Attribute 변경값
- 생성자에서 AttributeSet을 `CreateDefaultSubobject`로 생성
  → UE가 자동으로 ASC에 등록 (UAttributeSet 생성자 내부에서 처리)

---

## 네트워크 초기화 순서

```
[서버]
  Pawn Spawn
      │
  AMGCharacter::PossessedBy(Controller)
      │
  PS->GetMGAbilitySystemComponent()
      ->InitAbilityActorInfo(PS, Pawn)   ← Owner=PS, Avatar=Pawn

[소유 클라이언트]
  PlayerState replicate 수신
      │
  AMGCharacter::OnRep_PlayerState()
      │
  PS->GetMGAbilitySystemComponent()
      ->InitAbilityActorInfo(PS, Pawn)   ← 동일하게 호출
```

InitAbilityActorInfo를 두 번 호출하는 이유:
서버와 클라이언트에서 각각 타이밍이 달라서, 양쪽 모두에서 호출하지 않으면
한쪽에서 ASC가 Avatar를 모르는 상태가 된다.

---

## GameMode 연결

```cpp
// MGGameMode.cpp
AMGGameMode::AMGGameMode()
{
    PlayerStateClass = AMGPlayerState::StaticClass();
}
```

이 한 줄로 모든 플레이어가 AMGPlayerState를 갖게 되고,
AMGPlayerState 생성자에서 ASC + AttributeSet이 자동으로 생성된다.

---

## 에디터에서 해야 할 작업

1. **DA_PawnData_Player** 생성 (클래스: MGPawnData)
   - AnimClass, AbilitySet 나중에 채움

2. **DA_MGGameData** 열기 → DefaultPawnData 슬롯에 DA_PawnData_Player 할당

3. **DefaultGame.ini**에 PawnData 스캔 경로 추가:
   ```ini
   [/Script/MG.MGAssetManager]
   +PrimaryAssetTypesToScan=(PrimaryAssetType="MGPawnData",
     AssetBaseClass="/Script/MG.MGPawnData",
     bHasBlueprintClasses=False,
     bIsEditorOnly=False,
     Directories=((Path="/Game/MG/AbilitySystem")),
     SpecificAssets=,
     Rules=(Priority=-1,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
   ```

---

## 3단계 예고 (캐릭터 리팩터)

현재 AMGCharacter는 GetAbilitySystemComponent()에서 PlayerState ASC를 반환한다.
3단계에서 AMGPlayerCharacter / AMGEnemyCharacter로 분리되면:

- AMGPlayerCharacter → PlayerState ASC (현재와 동일)
- AMGEnemyCharacter → 자체 ASC + AttributeSet 소유 (Minimal 복제 모드)
- UMGPawnExtensionComponent → PawnData 로딩 + AbilitySet 부여 자동화
