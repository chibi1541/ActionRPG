// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ExecCalc/ARExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARExecCalc_Damage)

struct FARDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( AttackDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Defence );

	FARDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARAttackAttribSet, AttackDamage, Source, true );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARVitRefAttribSet, Defence, Target, false );
	}
};

static const FARDamageStatics& DamageStatics()
{
	static FARDamageStatics DStatics;
	return DStatics;
}


UARExecCalc_Damage::UARExecCalc_Damage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	RelevantAttributesToCapture.Add( DamageStatics().AttackDamageDef );
	RelevantAttributesToCapture.Add( DamageStatics().DefenceDef );
}

void UARExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().AttackDamageDef, EvaluationParameters, AttackDamage );

	FActionRPGGlobalTags Tags = FActionRPGGlobalTags::Get();
	float ExtraDamage = 0.f;
	ExtraDamage = Spec.GetSetByCallerMagnitude( Tags.ExtraDamageTag, false, 0.f );
	AttackDamage += ExtraDamage;

	float Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( DamageStatics().DefenceDef, EvaluationParameters, Defence );
	Defence = FMath::Max<float>( Defence, 0.0f );

	float MitigatedDamage = AttackDamage * ( 100 / ( 100 + Defence ) );

	if( MitigatedDamage > 0.f )
	{
		OutExecutionOutput.AddOutputModifier( FGameplayModifierEvaluatedData( UARVitRefAttribSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, MitigatedDamage ) );
	}
}
