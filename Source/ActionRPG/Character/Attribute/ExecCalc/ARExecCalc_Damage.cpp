// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ExecCalc/ARExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARExecCalc_Damage)

struct FARDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldGauge);

	FARDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARAttackAttribSet, AttackDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARVitRefAttribSet, Defence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARVitRefAttribSet, ShieldGauge, Target, false);
	}
};

static const FARDamageStatics& DamageStatics()
{
	static FARDamageStatics DStatics;
	return DStatics;
}


UARExecCalc_Damage::UARExecCalc_Damage(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShieldGaugeDef);
}

void UARExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AttackDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDamageDef, EvaluationParameters, AttackDamage);

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	float ExtraDamage = 0.f;
	ExtraDamage = Spec.GetSetByCallerMagnitude(Tags.ExtraDamageTag, false, 0.f);
	AttackDamage += ExtraDamage;

	float Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceDef, EvaluationParameters, Defence);
	Defence = FMath::Max<float>(Defence, 0.0f);

	float MitigatedDamage = AttackDamage * (100 / (100 + Defence));

	if (TargetAbilitySystemComponent->HasMatchingGameplayTag(Tags.AbilityStateTag_Guard))
	{
		float ShieldGauge = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldGaugeDef, EvaluationParameters, ShieldGauge);
		ShieldGauge = FMath::Max<float>(ShieldGauge, 0.0f);

		if (ShieldGauge >= MitigatedDamage)
		{
			// Hit Impact Cue Cancel
			OutExecutionOutput.MarkGameplayCuesHandledManually();
			FGameplayEventData Payload;
			Payload.ContextHandle = Spec.GetContext();
			TargetAbilitySystemComponent->HandleGameplayEvent(Tags.CharacterStateTag_Blocking, &Payload);
		}
	}

	if (MitigatedDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UARVitRefAttribSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, MitigatedDamage));
	}
}
