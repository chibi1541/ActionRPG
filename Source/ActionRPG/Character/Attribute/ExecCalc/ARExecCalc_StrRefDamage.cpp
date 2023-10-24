// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ExecCalc/ARExecCalc_StrRefDamage.h"

#include "AbilitySystemComponent.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARBaseAttribSet.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARExecCalc_StrRefDamage)

struct FARStrDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF( AttackDamage );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Defence );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Strength );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ShieldGauge );

	FARStrDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARBaseAttribSet, Strength, Source, true );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARVitRefAttribSet, Defence, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UARVitRefAttribSet, ShieldGauge, Target, false );
	}
};

static const FARStrDamageStatics& StrDamageStatics()
{
	static FARStrDamageStatics IDStatics;
	return IDStatics;
}

UARExecCalc_StrRefDamage::UARExecCalc_StrRefDamage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	RelevantAttributesToCapture.Add( StrDamageStatics().StrengthDef );
	RelevantAttributesToCapture.Add( StrDamageStatics().DefenceDef );
	RelevantAttributesToCapture.Add( StrDamageStatics().ShieldGaugeDef );
}


void UARExecCalc_StrRefDamage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
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

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	float BasicDamage = 0.f;
	BasicDamage = Spec.GetSetByCallerMagnitude( Tags.DamageTag, true, 0.f );

	float Strength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( StrDamageStatics().StrengthDef, EvaluationParameters, Strength );

	float ExtraDamageCoeff = 0.f;
	ExtraDamageCoeff = Spec.GetSetByCallerMagnitude( Tags.ExtraDamageCoeffTag, false, 0.f );

	float ModifieddDamage = BasicDamage + ( Strength * ExtraDamageCoeff );

	float Defence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( StrDamageStatics().DefenceDef, EvaluationParameters, Defence );
	Defence = FMath::Max<float>( Defence, 0.0f );

	float MitigatedDamage = ModifieddDamage * ( 100 / ( 100 + Defence ) );

	if( TargetAbilitySystemComponent->HasMatchingGameplayTag( Tags.AbilityStateTag_Guard ) )
	{
		float ShieldGauge = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( StrDamageStatics().ShieldGaugeDef, EvaluationParameters, ShieldGauge );
		ShieldGauge = FMath::Max<float>( ShieldGauge, 0.0f );

		if( ShieldGauge >= MitigatedDamage )
		{
			// Hit Impact Cue Cancel
			OutExecutionOutput.MarkGameplayCuesHandledManually();
			FGameplayEventData Payload;
			Payload.ContextHandle = Spec.GetContext();
			TargetAbilitySystemComponent->HandleGameplayEvent( Tags.CharacterStateTag_Blocking, &Payload );
		}
	}

	if( MitigatedDamage > 0.f )
	{
		OutExecutionOutput.AddOutputModifier( FGameplayModifierEvaluatedData( UARVitRefAttribSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, MitigatedDamage ) );
	}
}

