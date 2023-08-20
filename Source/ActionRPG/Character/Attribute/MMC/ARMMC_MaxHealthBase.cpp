// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_MaxHealthBase.h"

#include "Character/Attribute/ARPGBaseAttributeSet.h"
#include "Character/Attribute/ARPGVITAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_MaxHealthBase)

UARMMC_MaxHealthBase::UARMMC_MaxHealthBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	VitDef.AttributeToCapture = UARPGBaseAttributeSet::GetVitalityAttribute();
	VitDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitDef.bSnapshot = false;

	HealthRateDef.AttributeToCapture = UARPGVITAttributeSet::GetHealthRateAttribute();
	HealthRateDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HealthRateDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( VitDef );
	RelevantAttributesToCapture.Add( HealthRateDef );
}

float UARMMC_MaxHealthBase::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude( VitDef, Spec, EvaluationParameters, Vitality );

	float HealthRate = 0.f;
	GetCapturedAttributeMagnitude( HealthRateDef, Spec, EvaluationParameters, HealthRate );

	float MaxHealthBase = Vitality * HealthRate;

	return MaxHealthBase;
}
