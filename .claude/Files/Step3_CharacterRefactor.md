# Step 3 — 캐릭터 리팩터: ModularCharacter & PawnExtensionComponent

## 목표

UE 5.7 기본 템플릿의 단순한 `ACharacter` 기반 캐릭터를 Lyra 스타일의 **컴포넌트 기반 초기화 아키텍처**로 전환한다.  
카메라·입력·ASC 초기화 등의 책임을 명확히 분리하고, 플레이어와 AI 캐릭터가 공통 베이스를 공유하도록 한다.

---

## 새로 생성된 파일 목록

```
Source/MG/
├── MGGameplayTags.h / .cpp                          ← InitState 태그 정의
├── System/
│   └── MGGameInstance.h / .cpp                      ← InitState 태그 등록
└── Character/
    ├── MGCharacter.h / .cpp                         ← 추상 베이스
    ├── MGPlayerCharacter.h / .cpp                   ← 플레이어 전용
    ├── MGEnemyCharacter.h / .cpp                    ← AI 전용
    └── Components/
        └── MGPawnExtensionComponent.h / .cpp        ← 핵심 오케스트레이터
```

기존 `MGCharacter.h` (루트)는 `#include "Character/MGCharacter.h"` 로 redirect되었다.  
기존 `MGCharacter.cpp` (루트)는 삭제 (UBT 동명 파일 충돌 방지).

---

## 1. MGGameplayTags — InitState 태그

```cpp
// MGGameplayTags.h
namespace MGGameplayTags
{
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);
}
```

- `InitState.Spawned` — 액터가 월드에 존재함
- `InitState.DataAvailable` — PawnData가 설정됨
- `InitState.DataInitialized` — ASC가 초기화됨
- `InitState.GameplayReady` — 모든 피처가 준비됨

---

## 2. UMGGameInstance — InitState 등록

```
ProjectSettings → Maps & Modes → Game Instance Class = MGGameInstance
```

`UGameFrameworkComponentManager`는 GameInstance 서브시스템이다.  
게임 시작 시 한 번 상태 순서를 등록해야 모든 폰이 공유한다.

```cpp
void UMGGameInstance::Init()
{
    Super::Init();
    UGameFrameworkComponentManager* Manager = GetSubsystem<UGameFrameworkComponentManager>();
    Manager->RegisterInitState(Spawned,         false, FGameplayTag());   // 첫 번째
    Manager->RegisterInitState(DataAvailable,   false, Spawned);          // Spawned 다음
    Manager->RegisterInitState(DataInitialized, false, DataAvailable);    // DataAvailable 다음
    Manager->RegisterInitState(GameplayReady,   false, DataInitialized);  // DataInitialized 다음
}
```

`RegisterInitState(NewState, bAddBefore, ExistingState)` — `false` = ExistingState **뒤에** 삽입.

---

## 3. UMGPawnExtensionComponent — 핵심 오케스트레이터

### 역할

모든 `AMGCharacter`에 부착되는 컴포넌트.  
`IGameFrameworkInitStateInterface`를 구현해 InitState 상태 머신을 주도하고,  
PawnData를 복제하며, ASC 초기화/해제를 중앙에서 관리한다.

### 상태 전이 조건

```
[없음] ──────────────── Spawned          : Pawn != nullptr
Spawned ──────────────── DataAvailable   : PawnData != nullptr
DataAvailable ────────── DataInitialized : AbilitySystemComponent != nullptr
                                           && 모든 피처가 DataAvailable 도달
DataInitialized ──────── GameplayReady   : 모든 피처가 DataInitialized 도달
```

`HaveAllFeaturesReachedInitState(Pawn, Tag)` — UE 5.7 `UGameFrameworkComponentManager` API.  
이 조건 때문에 **ASC가 설정되기 전까지 DataInitialized로 절대 전진하지 않는다**.

### PawnData 흐름

```
서버                              클라이언트
────────────────────────          ────────────────────────
SetPawnData(InPawnData)           OnRep_PawnData() 호출됨
  → PawnData 설정                   → CheckDefaultInitialization()
  → ForceNetUpdate()
  → CheckDefaultInitialization()
```

- `SetPawnData()`는 서버 전용 (`ROLE_Authority` 체크 포함).
- `DefaultPawnData` (AMGCharacter의 `EditDefaultsOnly` 프로퍼티) 를 통해  
  Experience 시스템 없이도 BP에서 직접 PawnData를 지정할 수 있다.

### ASC 초기화/해제

```cpp
// 캐릭터 → 컴포넌트 호출
void InitializeAbilitySystem(UMGAbilitySystemComponent* InASC, AActor* InOwnerActor);
void UninitializeAbilitySystem();
```

- `InitAbilityActorInfo(InOwnerActor, Pawn)` 내부 호출
- 완료 후 `CheckDefaultInitialization()` → DataInitialized 전이 가능

### 주요 API

| 메서드 | 설명 |
|--------|------|
| `SetPawnData(Data)` | 서버에서 PawnData 주입 |
| `InitializeAbilitySystem(ASC, Owner)` | ASC 연결 + InitAbilityActorInfo |
| `UninitializeAbilitySystem()` | ASC 연결 해제 |
| `HandleControllerChanged()` | PossessedBy/UnPossessed에서 호출 |
| `HandlePlayerStateReplicated()` | OnRep_PlayerState에서 호출 |
| `OnAbilitySystemInitialized` | ASC 준비 완료 시 브로드캐스트 |
| `OnAbilitySystemUninitialized` | ASC 해제 시 브로드캐스트 |

### IGameFrameworkInitStateInterface 구현 (UE 5.7 기준)

```cpp
// UE 5.7 실제 메서드명 — Lyra와 다름!
CheckDefaultInitializationForImplementers();   // (Lyra: ForAllFeatures)
GetComponentManager();                          // (Lyra: GetGameFrameworkComponentManager)
ContinueInitStateChain({...});                 // 동일
```

---

## 4. AMGCharacter — 추상 베이스

### 변경 사항 (기존 대비)

| 항목 | 기존 | 변경 후 |
|------|------|---------|
| 부모 | `ACharacter` | `ACharacter` (동일, ModularGameplay는 컴포넌트로) |
| 카메라 | `CameraBoom`, `FollowCamera` 직접 소유 | 제거 → AMGPlayerCharacter로 이동 |
| 입력 | `JumpAction`, `MoveAction` 등 직접 바인딩 | 제거 → Step 4에서 InputConfig 방식으로 |
| ASC 접근 | `GetPlayerState<AMGPlayerState>()->GetASC()` | `PawnExtensionComponent->GetMGASC()` |
| PawnData | 없음 | `DefaultPawnData` (EditDefaultsOnly) 추가 |

### 소유 컴포넌트

```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ...)
TObjectPtr<UMGPawnExtensionComponent> PawnExtensionComponent;
```

### BeginPlay (서버)

```cpp
// Experience 시스템 없이 동작할 때 BP에서 지정한 DefaultPawnData를 PawnExtension에 주입
if (HasAuthority() && DefaultPawnData && !PawnExtensionComponent->GetPawnData())
{
    PawnExtensionComponent->SetPawnData(DefaultPawnData);
}
```

### PossessedBy / UnPossessed / OnRep_PlayerState

```cpp
void AMGCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    PawnExtensionComponent->HandleControllerChanged();
}

void AMGCharacter::UnPossessed()
{
    Super::UnPossessed();
    PawnExtensionComponent->UninitializeAbilitySystem(); // 언포세스 시 ASC 해제
    PawnExtensionComponent->HandleControllerChanged();
}

void AMGCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    PawnExtensionComponent->HandlePlayerStateReplicated();
}
```

---

## 5. AMGPlayerCharacter — 플레이어 전용

### 역할

- **카메라 rig** (`USpringArmComponent` + `UCameraComponent`) 소유
- `PossessedBy` / `OnRep_PlayerState` 에서 PlayerState의 ASC를 PawnExtension에 연결

### ASC 초기화 경로

```
[서버] PossessedBy() → InitializePlayerASC()
[클라이언트] OnRep_PlayerState() → InitializePlayerASC()

InitializePlayerASC():
  PlayerState = GetPlayerState<AMGPlayerState>()
  PawnExtension->InitializeAbilitySystem(PS->GetMGASC(), PS)
       │
       └── InitAbilityActorInfo(OwnerActor=PS, AvatarActor=Pawn)
           OwnerActor = PlayerState  (ASC 소유자)
           AvatarActor = 이 캐릭터   (어빌리티 실행 주체)
```

플레이어 ASC는 **PlayerState가 소유** — GAS 멀티플레이어 표준 패턴.

### 입력 (Step 4에서 추가 예정)

현재 `SetupPlayerInputComponent`는 빈 override.  
Step 4에서 `UMGInputComponent` + `UMGInputConfig` + GAS 연동 추가.

---

## 6. AMGEnemyCharacter — AI 전용

### 역할

- ASC를 **자기 자신 (Pawn)** 이 직접 소유
- `BeginPlay`에서 즉시 `InitializeAbilitySystem` 호출

### ASC 초기화 경로

```
BeginPlay()
  → Super::BeginPlay()  (DefaultPawnData 주입)
  → PawnExtension->InitializeAbilitySystem(AbilitySystemComponent, this)
         │
         └── InitAbilityActorInfo(OwnerActor=this, AvatarActor=this)
             양쪽 모두 자기 자신 — AI 표준 패턴
```

### 복제 모드

```cpp
AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
```

- `Minimal` — 서버가 권위적으로 처리, 클라이언트 예측 없음
- 플레이어는 `Mixed` 또는 `Full` (Step 4에서 설정)

### 소유 AttributeSet

```cpp
TObjectPtr<UMGHealthSet> HealthSet;   // 체력
TObjectPtr<UMGCombatSet> CombatSet;  // 공격력, 방어력, 크리티컬
// StaminaSet 없음 — AI는 스태미나 불필요
```

---

## 7. 전체 초기화 시퀀스

### 플레이어 캐릭터 (서버)

```
World::SpawnActor<AMGPlayerCharacter>
  └── 생성자: PawnExtensionComponent 생성
         ↓
OnRegister(): RegisterInitStateFeature()
         ↓
BeginPlay():
  ├── SetPawnData(DefaultPawnData)  → 상태: Spawned → DataAvailable
  └── PawnExtension::BeginPlay()
        ├── BindOnActorInitStateChanged()
        └── TryToChangeInitState(Spawned)
         ↓
GameMode::RestartPlayer() → PossessedBy(PlayerController)
  ├── PlayerState 연결됨
  ├── PawnExtension::HandleControllerChanged() → CheckDefaultInitialization()
  └── InitializePlayerASC()
        └── PawnExtension::InitializeAbilitySystem(PS->ASC, PS)
              ├── InitAbilityActorInfo(PS, Character)
              ├── OnAbilitySystemInitialized.Broadcast()
              └── CheckDefaultInitialization()
                    → DataAvailable → DataInitialized → GameplayReady ✓
```

### 플레이어 캐릭터 (클라이언트)

```
OnRep_PlayerState() 수신
  ├── PawnExtension::HandlePlayerStateReplicated()
  └── InitializePlayerASC()
        └── (서버와 동일한 InitializeAbilitySystem 흐름)

OnRep_PawnData() 수신 (서버에서 복제)
  └── PawnExtension::CheckDefaultInitialization()
```

### AI/Enemy 캐릭터

```
World::SpawnActor<AMGEnemyCharacter>
  └── 생성자: ASC + AttributeSet 생성
         ↓
BeginPlay():
  ├── Super::BeginPlay() → SetPawnData(DefaultPawnData)
  │                      → Spawned → DataAvailable
  └── InitializeAbilitySystem(자체ASC, this)
        → DataAvailable → DataInitialized → GameplayReady ✓
```

---

## 8. 플레이어 vs AI 구조 비교

| 항목 | AMGPlayerCharacter | AMGEnemyCharacter |
|------|-------------------|-------------------|
| ASC 소유자 | AMGPlayerState | AMGEnemyCharacter (self) |
| ASC 복제 모드 | Mixed (Step 4) | Minimal |
| InitAbilityActorInfo | (PlayerState, Character) | (self, self) |
| ASC 초기화 시점 | PossessedBy / OnRep_PlayerState | BeginPlay |
| 클라이언트 예측 | ON (Step 4~) | OFF |
| AttributeSet 소유 | AMGPlayerState | AMGEnemyCharacter |

---

## 9. 다음 단계 (Step 4)

- `UMGInputConfig` — GA 태그 → InputAction 매핑 DataAsset
- `UMGInputComponent` — EnhancedInputComponent 서브클래스, GAS와 연동
- `AMGPlayerCharacter::SetupPlayerInputComponent()` 구현
- `AbilitySet`을 PawnData로부터 읽어 어빌리티 자동 부여
