# ActionRPG

> **Unreal Engine 5 기반 액션 RPG 포트폴리오 프로젝트**
> Gameplay Ability System(GAS)을 중심으로 어빌리티 · 속성(Attribute) · AI · UI를 모듈화하여 설계한 C++ 게임 프레임워크입니다.

이 프로젝트는 상용 액션 RPG에서 요구되는 핵심 게임플레이 시스템을 **데이터 주도(Data-Driven)** 방식과 **태그 기반 설계**로 직접 구현하는 데 초점을 맞춘 학습/포트폴리오 결과물입니다. 단순 기능 구현을 넘어, 엔진의 GAS · Enhanced Input · Behavior Tree · CommonUI 등을 확장하여 재사용 가능한 구조로 통합한 점이 특징입니다.

---

## 📌 핵심 특징 (Highlights)

| 영역 | 구현 내용 |
|------|-----------|
| **Gameplay Ability System** | `UAbilitySystemComponent`를 확장한 `UARAbilitySystemComponent`로 입력 태그 기반 어빌리티 활성화, 태그 관계(Block/Cancel/Required) 테이블 처리 |
| **어빌리티 모듈화** | `UAbilitySet` 데이터 애셋으로 어빌리티 묶음을 캐릭터에 부여(Grant). 블링크 · 캐스팅 · 콤보 공격 · 스태미나 소모 등 어빌리티를 클래스 단위로 분리 |
| **속성(Attribute) 시스템** | STR/AGI/INT/VIT 기본 스탯을 기반으로 한 다층 AttributeSet 구조. MMC(ModifierMagnitudeCalculation) · ExecCalc로 데미지·회복·스탯 파생값을 계산 |
| **콤보 공격** | `UARComboAttackComponent` + GameplayEvent 기반 몽타주 시퀀스로 입력 선행 입력(다음 공격 예약) 처리 |
| **AI** | Behavior Tree + EQS + 커스텀 **Utility 기반 의사결정 플러그인(BTUtilityPlugin)**으로 몬스터 행동 제어. 태그로 어빌리티를 활성화하는 BTTask 포함 |
| **UI 프레임워크** | CommonUI 기반 `UARGameUIManagerSubsystem`의 Extension Point 시스템으로 HUD 위젯을 태그로 동적 등록/해제 |
| **메시지 시스템** | 채널(GameplayTag) 기반 `UARGameplayMessageSubsystem`으로 시스템 간 느슨한 결합(Loose Coupling) 통신. 전용 K2 블루프린트 노드 제공 |
| **데이터 파이프라인** | `UARJsonFormDataAsset` + `UARCustomDevSettings`로 데이터 애셋을 JSON으로 Import/Export (에디터 전용) |

---

## 🛠 기술 스택 (Tech Stack)

- **엔진**: Unreal Engine **5.7**
- **언어**: C++ (런타임) + Blueprint (조립/연출)
- **주요 모듈/플러그인**:
  `GameplayAbilities` · `EnhancedInput` · `AIModule` · `NavigationSystem` · `CommonUI` · `GameplayTags` · `DeveloperSettings` · `Json`/`JsonUtilities`
- **자체 플러그인**: `BTUtilityPlugin` (Behavior Tree Node 기반 Utility AI 시스템)

---

## 🧱 아키텍처 개요

게임플레이는 **GAS를 허브**로 하여 각 시스템이 GameplayTag와 GameplayEffect를 매개로 상호작용합니다.

```
                          ┌─────────────────────────────┐
            Enhanced      │   UARAbilitySystemComponent  │
            Input  ──────▶│  (입력 태그 → 어빌리티 활성화)  │
         (InputTag)       │   + TagRelationship 테이블     │
                          └──────────────┬──────────────┘
                                         │ grant / activate
                  ┌──────────────────────┼───────────────────────┐
                  ▼                       ▼                        ▼
          ┌───────────────┐     ┌──────────────────┐     ┌────────────────┐
          │  UAbilitySet  │     │  ARGameplayAbility │     │  AttributeSets  │
          │ (어빌리티 묶음) │     │ (Blink/Casting/   │     │ Base→Attack/Vit │
          │   DataAsset    │     │  Combo/Cooldown…) │     │ /Agi/Int Ref…   │
          └───────────────┘     └────────┬──────────┘     └───────┬────────┘
                                          │ apply GE                │ MMC / ExecCalc
                                          ▼                         ▼
                                 ┌──────────────────┐     ┌────────────────┐
                                 │  ARGameplayEffect │────▶│  Damage / Heal  │
                                 └──────────────────┘     │  계산 (파생 스탯) │
                                                          └────────────────┘

   [AI]  BaseAIController ─▶ Behavior Tree (+ EQS + BTUtilityPlugin)
                                   └─▶ BTTask_ActiveAbilityByTag ─▶ ASC

   [UI]  ARGameUIManagerSubsystem (CommonUI) ─ Extension Point(Tag) ─▶ HUD 위젯
   [Msg] ARGameplayMessageSubsystem ─ Channel(Tag) ─▶ 시스템 간 Pub/Sub
```

### 캐릭터 클래스 계층

```
ACharacter (Engine)
└── ABaseCharacter            // ASC · AttributeSet · 상태/타게팅 컴포넌트 보유
    ├── AHeroCharacter        // 플레이어: 카메라, Enhanced Input, 콤보, HUD, HeroClass
    └── ABaseMonster          // 몬스터: 플로팅 HP바, 사망 몽타주, MonsterType
```

---

## 📂 폴더 구조

### 소스 (`Source/`)

```
Source/
├── ActionRPG/                      # 런타임 게임 모듈
│   ├── ActionRPG_Lib.h             # 공용 로그/단언(RCHECK)/Enum 매크로
│   ├── ARGameInstance / ARGameState / ActionRPGGameMode
│   ├── ARCustomDevSettings         # 프로젝트 설정(데이터 Import/Export 경로 등)
│   ├── ARJsonFormDataAsset         # DataAsset ↔ JSON 변환 베이스
│   │
│   ├── Ability/                    # GAS 어빌리티 계층
│   │   ├── ARAbilitySystemComponent     # 입력 태그 처리, 태그 관계 적용
│   │   ├── ARGameplayAbility            # 어빌리티 베이스(활성화 정책)
│   │   ├── ARGA_*                       # Blink / Casting / Combo / Cooldown 등 구현
│   │   ├── AbilitySet · GameplayEffectSet · TagRelationship   # 데이터 애셋
│   │   ├── HeroInputConfig              # InputTag ↔ InputAction 매핑
│   │   ├── AbilityTask/                 # 커스텀 몽타주 재생 Task
│   │   └── GameplayEffect/              # ARGameplayEffect
│   │
│   ├── Character/                  # 캐릭터 & 컴포넌트
│   │   ├── BaseCharacter / HeroCharacter / BaseMonster
│   │   ├── ARPlayerController · BaseAIController · ARProjectile
│   │   ├── HeroInputComponent / *MovementComponent
│   │   ├── Attribute/                   # AttributeSet 계열
│   │   │   ├── ARBaseAttribSet          # STR/AGI/INT/VIT/Level/MoveSpeed
│   │   │   ├── AR{Attack,Vit,Agi,Int}RefAttribSet
│   │   │   ├── ExecCalc/                # 데미지 계산(GameplayEffectExecutionCalculation)
│   │   │   └── MMC/                     # 파생 스탯 계산(ModifierMagnitudeCalculation)
│   │   └── Components/                  # Combo / Targeting / GetHit / HitTrace / State
│   │
│   ├── AI/                         # Behavior Tree Task/Service, EQS Context
│   ├── GameFactor/                 # 몬스터 스포너 & 스포너 매니저
│   ├── Subsystem/                  # GameInstanceSubsystem 계열
│   │   ├── Attribute/                   # 클래스별 스탯 계수 테이블
│   │   ├── GameplayMessage/             # 태그 채널 메시지 Pub/Sub
│   │   └── UserInterface/               # CommonUI 레이아웃/Extension 매니저
│   └── UserInterface/              # HUD/위젯(보스 HP, 상태창 등)
│
└── ActionRPGEditor/                # 에디터 전용 모듈
    └── GameplayMessageNodes/       # 메시지 구독용 커스텀 K2 블루프린트 노드
```

### 플러그인 (`Plugins/`)

| 플러그인 | 설명 |
|----------|------|
| **BTUtilityPlugin** | Behavior Tree 노드 기반 **Utility AI** 시스템. `BTComposite_Utility`(우선순위/비례 선택)와 `BTDecorator_Utility*`(Blackboard·Curve·Function 기반 점수 계산) 제공 |

### 콘텐츠 (`Content/`)

| 폴더 | 내용 |
|------|------|
| `GAS/` | GameplayAbilities · GameplayEffects · GameplayCue · Attribute 애셋 |
| `DataAssets/` | AbilitySet · Attribute · Curve · EffectSet · UtilityCurve 데이터 애셋 |
| `Characters/`, `Paragon*` | 플레이어/몬스터 메시 및 애니메이션 (Paragon 에셋 활용) |
| `Input/` | InputMappingContext · InputAction · HeroInputConfig |
| `UserInterface/` | HUD/위젯 블루프린트 (`W_PrimaryLayout` 등) |
| `Level/` | 플레이 가능한 레벨 (`Level1` = 기본 시작 맵) |
| `Blueprints/` | 게임 인스턴스 등 핵심 BP (`ARGameInstance_BP`) |

> `Data/DataAssets/`에는 JSON Import/Export 파이프라인으로 관리되는 외부 데이터 리소스가 위치합니다.

---

## 🎮 조작 방법

입력은 **Enhanced Input**으로 처리되며, `InputAction`이 `InputTag`에 매핑되어 GAS 어빌리티를 활성화합니다.
구체적인 키 바인딩은 `Content/Input/`의 InputMappingContext와 `HeroInputConfig` 애셋에서 확인/수정할 수 있습니다.

---

## 📝 참고

- 본 프로젝트는 **포트폴리오 목적**으로 제작되었으며, 일부 콘텐츠 에셋(Paragon, Infinity Blade, Starter Content 등)은 Epic Games에서 제공하는 무료 에셋을 사용합니다.
- Unreal **5.1 → 5.7** 엔진 업그레이드 및 에셋/코드 구조 정리 작업이 진행되었습니다.
