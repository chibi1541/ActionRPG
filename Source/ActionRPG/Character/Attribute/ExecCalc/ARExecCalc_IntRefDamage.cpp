// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ExecCalc/ARExecCalc_IntRefDamage.h"

#include "AbilitySystemComponent.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARBaseAttribSet.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARExecCalc_IntRefDamage)

struct FARIntDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( AttackDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Defence );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Intelligence );

	FARIntDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARAttackAttribSet, AttackDamage, Source, true );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARBaseAttribSet, Intelligence, Source, true );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARVitRefAttribSet, Defence, Target, false );
	}
};

static const FARIntDamageStatics& IntDamageStatics()
{
	static FARIntDamageStatics IDStatics;
	return IDStatics;
}

UARExecCalc_IntRefDamage::UARExecCalc_IntRefDamage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	RelevantAttributesToCapture.Add( IntDamageStatics().AttackDamageDef );
	RelevantAttributesToCapture.Add( IntDamageStatics().IntelligenceDef );
	RelevantAttributesToCapture.Add( IntDamageStatics().DefenceDef );
}


void UARExecCalc_IntRefDamage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
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

	float BasicDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( IntDamageStatics().AttackDamageDef, EvaluationParameters, BasicDamage );

	float Intelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( IntDamageStatics().IntelligenceDef, EvaluationParameters, Intelligence );

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	float ExtraDamageCoeff = 0.f;
	ExtraDamageCoeff = Spec.GetSetByCallerMagnitude( Tags.ExtraDamageCoeffTag, false, 0.f );

	float ModifieddDamage = BasicDamage + ( Intelligence * ExtraDamageCoeff );

	float Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( IntDamageStatics().DefenceDef, EvaluationParameters, Defence );
	Defence = FMath::Max<float>( Defence, 0.0f );

	float MitigatedDamage = ModifieddDamage * ( 100 / ( 100 + Defence ) );

	if( MitigatedDamage > 0.f )
	{
		OutExecutionOutput.AddOutputModifier( FGameplayModifierEvaluatedData( UARVitRefAttribSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, MitigatedDamage ) );
	}
}
