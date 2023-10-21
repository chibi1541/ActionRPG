// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_ShieldGaugeRegene.h"

#include "Character/Attribute/ARVitRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_ShieldGaugeRegene)

UARMMC_ShieldGaugeRegene::UARMMC_ShieldGaugeRegene( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	MaxShieldGaugeAttribDef.AttributeToCapture = UARVitRefAttribSet::GetMaxShieldGaugeAttribute();
	MaxShieldGaugeAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxShieldGaugeAttribDef.bSnapshot = false;

	ShieldRegeneAttribDef.AttributeToCapture = UARVitRefAttribSet::GetShieldGaugeRegeneAttribute();
	ShieldRegeneAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ShieldRegeneAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( MaxShieldGaugeAttribDef );
	RelevantAttributesToCapture.Add( ShieldRegeneAttribDef );
}

float UARMMC_ShieldGaugeRegene::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxShieldGauge = 0.f;
	GetCapturedAttributeMagnitude( MaxShieldGaugeAttribDef, Spec, EvaluationParameters, MaxShieldGauge );
	MaxShieldGauge = FMath::Max( MaxShieldGauge, 0.f );

	float ShieldRegene = 0.f;
	GetCapturedAttributeMagnitude( ShieldRegeneAttribDef, Spec, EvaluationParameters, ShieldRegene );
	ShieldRegene = FMath::Max( ShieldRegene, 0.f );

	float RegenPoint = MaxShieldGauge * ShieldRegene;

	return RegenPoint;
}