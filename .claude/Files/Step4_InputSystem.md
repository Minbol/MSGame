# Step 4 — 입력 시스템: MGInputConfig + EnhancedInput → GAS 연동

## 목표

EnhancedInput 기반의 입력 시스템을 구축하고 GAS 어빌리티와 연동한다.  
GameplayTag를 InputAction의 키로 사용해 하드코딩 없이 어빌리티를 활성화한다.  
PawnData의 AbilitySet을 ASC 초기화 완료 시점에 자동으로 부여한다.

---

## 새로 생성된 파일 목록

```
Source/MG/
├── MGGameplayTags.h / .cpp      ← Input 태그 9개 추가
└── Input/
    ├── MGInputConfig.h / .cpp   ← DataAsset: Tag → InputAction 매핑
    └── MGInputComponent.h / .cpp ← EnhancedInputComponent 서브클래스
```

수정된 파일:
```
AbilitySystem/MGPawnData.h                     ← InputConfig 필드 추가
AbilitySystem/Components/MGAbilitySystemComponent.h/.cpp  ← Tag 기반 입력 처리
Character/MGPlayerCharacter.h/.cpp             ← 입력 바인딩 + AbilitySet 부여
```

---

## 1. MGGameplayTags — Input 태그

### Native 태그 (GAS 없이 직접 바인딩)

```
MG.Input.Move     — 2D 축 입력 → AddMovementInput
MG.Input.Look     — 2D 축 입력 → AddControllerPitchInput / YawInput
MG.Input.Sprint   — 스프린트 (향후 구현)
```

### Ability 태그 (GAS 어빌리티 활성화)

```
MG.Input.Jump
MG.Input.Dodge
MG.Input.Attack.Light
MG.Input.Attack.Heavy
MG.Input.Parry
MG.Input.Interact
```

어빌리티 부여 시 `FGameplayAbilitySpec.DynamicAbilityTags`에 이 태그를 추가해 둔다 (AbilitySet.cpp에서 이미 처리).

---

## 2. UMGInputConfig — DataAsset

### 역할

GameplayTag → InputAction 매핑을 보유하는 DataAsset.  
`DA_InputConfig_Player` 같은 에셋을 하나 만들어 `UMGPawnData.InputConfig`에 연결한다.

### 구조

```cpp
UPROPERTY(EditDefaultsOnly, Category="Input|Native")
TArray<FMGInputAction> NativeInputActions;   // Move, Look, Sprint

UPROPERTY(EditDefaultsOnly, Category="Input|Ability")
TArray<FMGInputAction> AbilityInputActions;  // Jump, Dodge, Attack…
```

`FMGInputAction`은 `FGameplayTag InputTag` + `UInputAction* InputAction` 쌍.

### API

```cpp
const UInputAction* FindNativeInputActionForTag(Tag, bLogNotFound=true) const;
const UInputAction* FindAbilityInputActionForTag(Tag, bLogNotFound=true) const;
```

---

## 3. UMGInputComponent — EnhancedInputComponent 서브클래스

### 역할

`UEnhancedInputComponent`를 상속해 두 헬퍼 템플릿을 추가한다.

### API

```cpp
// 하나의 Native 액션을 태그로 바인딩 (일반 멤버 함수)
template<class UserClass, typename FuncType>
void BindNativeAction(Config, InputTag, TriggerEvent, Object, Func);

// 모든 Ability 액션을 한 번에 바인딩 (콜백에 InputTag 전달)
// PressedFunc / ReleasedFunc 시그니처: void(FGameplayTag)
template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void BindAbilityActions(Config, Object, PressedFunc, ReleasedFunc, OutBindingHandles);
```

`BindAbilityActions` 내부에서 각 AbilityInputAction마다:
- `ETriggerEvent::Triggered` → PressedFunc(InputTag) 바인딩
- `ETriggerEvent::Completed` → ReleasedFunc(InputTag) 바인딩

바인딩 핸들은 `OutBindingHandles`에 저장 → uninit 시 `RemoveBindingByHandle`로 해제.

### 프로젝트 세팅

```
Project Settings → Input → Default Input Component Class = MGInputComponent
```

---

## 4. UMGPawnData — InputConfig 추가

```cpp
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
TObjectPtr<const UMGInputConfig> InputConfig;   // AI 폰은 null
```

---

## 5. UMGAbilitySystemComponent — Tag 기반 입력 처리

### 새로운 메서드

```cpp
void AbilityInputTagPressed(const FGameplayTag& InputTag);
void AbilityInputTagReleased(const FGameplayTag& InputTag);
void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
void ClearAbilityInput();
```

### 내부 배열

```cpp
TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
```

### 흐름

```
[입력 이벤트] ETriggerEvent::Triggered 발생
  → Input_AbilityInputTagPressed(InputTag) 호출
  → AbilityInputTagPressed(InputTag)
      ActivatableAbilities 순회
      DynamicAbilityTags.HasTagExact(InputTag) 인 Spec 찾음
      → InputPressedSpecHandles.AddUnique(Spec.Handle)
      → InputHeldSpecHandles.AddUnique(Spec.Handle)

[Tick] AMGPlayerCharacter::Tick (IsLocallyControlled일 때만)
  → ASC->ProcessAbilityInput(DeltaTime, false)
      InputPressedSpecHandles 순회:
        Spec이 비활성 → TryActivateAbility(Handle)  (클라이언트 예측)
        Spec이 활성   → AbilitySpecInputPressed(Spec)  (WaitInputPress Task 지원)
      InputReleasedSpecHandles 순회:
        → AbilitySpecInputReleased(Spec)  (WaitInputRelease Task 지원)
      두 배열 Reset()

[입력 해제] ETriggerEvent::Completed 발생
  → AbilityInputTagReleased(InputTag)
      → InputReleasedSpecHandles.AddUnique
      → InputHeldSpecHandles.Remove
```

### DynamicAbilityTags 연결 시점

`UMGAbilitySet::GiveToAbilitySystem()`에서 어빌리티 부여 시 (Step 3에서 이미 구현됨):

```cpp
FGameplayAbilitySpec Spec(AbilityCDO, Level);
Spec.DynamicAbilityTags.AddTag(AbilityEntry.InputTag);   // ← 이 태그로 입력 라우팅
ASC->GiveAbility(Spec);
```

---

## 6. AMGPlayerCharacter 변경 사항

### BeginPlay — Delegate 구독 (1회)

```cpp
void AMGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    UMGPawnExtensionComponent* PawnExt = GetPawnExtensionComponent();
    PawnExt->OnAbilitySystemInitialized.AddUObject(this, &ThisClass::OnAbilitySystemInitialized);
    PawnExt->OnAbilitySystemUninitialized.AddUObject(this, &ThisClass::OnAbilitySystemUninitialized);
}
```

BeginPlay에서 1회만 바인딩 → `PossessedBy` / `OnRep_PlayerState` 양쪽에서 `InitializePlayerASC`가 호출돼도 중복 바인딩 없음.

### OnAbilitySystemInitialized — AbilitySet 부여

```cpp
void AMGPlayerCharacter::OnAbilitySystemInitialized()
{
    // PawnData→AbilitySet을 ASC에 부여, 핸들 저장
    if (PawnData && PawnData->AbilitySet)
    {
        PawnData->AbilitySet->GiveToAbilitySystem(ASC, &AbilitySetHandles, this);
    }
}
```

- 서버에서만 실질적으로 부여 (`GiveToAbilitySystem` 내부 `IsOwnerActorAuthoritative()` 체크).
- 클라이언트는 GAS 복제로 Spec을 수신.

### OnAbilitySystemUninitialized — 정리

```cpp
void AMGPlayerCharacter::OnAbilitySystemUninitialized()
{
    AbilitySetHandles.TakeFromAbilitySystem(ASC);    // 어빌리티 해제

    // 입력 바인딩 해제 (스테일 ASC로 이벤트 전달 방지)
    for (uint32 Handle : AbilityInputBindingHandles)
        InputComponent->RemoveBindingByHandle(Handle);
    AbilityInputBindingHandles.Reset();
}
```

### SetupPlayerInputComponent — 입력 바인딩

```cpp
void AMGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // 1. 매핑 컨텍스트 초기화 (실제 컨텍스트 추가는 BP 또는 Experience에서)
    Subsystem->ClearAllMappings();

    // 2. Native 바인딩
    MGIC->BindNativeAction(InputConfig, InputTag_Move, ETriggerEvent::Triggered, this, &Input_Move);
    MGIC->BindNativeAction(InputConfig, InputTag_Look, ETriggerEvent::Triggered, this, &Input_Look);

    // 3. Ability 바인딩 (모든 AbilityInputActions 일괄)
    MGIC->BindAbilityActions(InputConfig, this,
        &Input_AbilityInputTagPressed,
        &Input_AbilityInputTagReleased,
        AbilityInputBindingHandles);
}
```

### Tick — ProcessAbilityInput

```cpp
void AMGPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (IsLocallyControlled())
        ASC->ProcessAbilityInput(DeltaSeconds, false);
}
```

---

## 7. 전체 데이터 흐름

```
에디터 설정:
  DA_InputConfig_Player
    NativeInputActions:  [ (MG.Input.Move, IA_Move), (MG.Input.Look, IA_Look) ]
    AbilityInputActions: [ (MG.Input.Jump, IA_Jump), (MG.Input.Attack.Light, IA_Attack_Light), … ]

  DA_PawnData_Player
    AbilitySet = DA_AbilitySet_Player
    InputConfig = DA_InputConfig_Player

  DA_AbilitySet_Player
    GrantedAbilities: [ (GA_Jump, Level=1, InputTag=MG.Input.Jump), … ]

런타임:
  BeginPlay → PawnData 주입 (서버) → Spawned→DataAvailable
  PossessedBy → ASC 연결 → DataInitialized→GameplayReady
              → OnAbilitySystemInitialized.Broadcast()
                → AbilitySet.GiveToAbilitySystem()
                    Spec.DynamicAbilityTags = [MG.Input.Jump]
                    ASC에 어빌리티 부여
  SetupPlayerInputComponent → InputConfig 바인딩
  플레이어가 Jump 버튼 누름
    → Input_AbilityInputTagPressed(MG.Input.Jump)
    → ASC.InputPressedSpecHandles += [JumpSpec.Handle]
  Tick
    → ProcessAbilityInput
    → TryActivateAbility(JumpSpec.Handle) → GA_Jump 실행
```

---

## 8. 에디터 세팅 체크리스트

- [ ] `Project Settings → Input → Default Input Component Class = MGInputComponent`
- [ ] `IA_Move`, `IA_Look`, `IA_Jump` 등 InputAction 에셋 생성
- [ ] `IMC_Default` (InputMappingContext) 생성 후 각 IA에 키 매핑
- [ ] `DA_InputConfig_Player` 생성, 위 태그-IA 연결
- [ ] `BP_MGPlayerCharacter` 또는 `SetupPlayerInputComponent` 이후 IMC 추가 로직 구현
  - 현재 `SetupPlayerInputComponent`에서 `ClearAllMappings()`만 호출, IMC 추가는 BP에서 수행
- [ ] `DA_PawnData_Player.InputConfig = DA_InputConfig_Player` 설정
- [ ] `DA_AbilitySet_Player` 각 어빌리티 항목에 `InputTag` 설정

---

## 9. 알려진 제한 사항

**SetupPlayerInputComponent 시점의 PawnData null 문제:**  
클라이언트에서 `SetupPlayerInputComponent`가 호출될 때 `OnRep_PawnData`가 아직 도달하지 않으면 InputConfig가 null이 되어 바인딩이 스킵될 수 있다.  
→ Lyra는 별도의 `ULyraHeroComponent`(IGameFrameworkInitStateInterface 구현)에서 `GameplayReady` 전이 시점에 입력 바인딩을 수행해 이 문제를 해결한다.  
→ Step 8(GameFeatures/Experience)에서 HeroComponent로 이전 예정.

---

## 10. 다음 단계 (Step 5)

- `UMGGameplayAbility` 베이스에 `ActivationPolicy` (OnInputTriggered / WhileInputActive) 추가
- `UMGAbility_Jump`, `UMGAbility_Dodge`, `UMGAbility_LightAttack`, `UMGAbility_HeavyAttack` 구현
- `WaitInputPress` / `WaitInputRelease` AbilityTask로 콤보 트리거
