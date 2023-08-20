// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_MaxShieldGaugeBase.h"

#include "Character/Attribute/ARPGBaseAttributeSet.h"
#include "Character/Attribute/ARPGVITAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_MaxShieldGaugeBase)

UARMMC_MaxShieldGaugeBase::UARMMC_MaxShieldGaugeBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	
	VitDef.AttributeToCapture = UARPGBaseAttributeSet::GetVitalityAttribute();
	VitDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitDef.bSnapshot = false;

	ShieldGaugeRateDef.AttributeToCapture = UARPGVITAttributeSet::GetShieldGaugeRateAttribute();
	ShieldGaugeRateDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ShieldGaugeRateDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( VitDef );
	RelevantAttributesToCapture.Add( ShieldGaugeRateDef );
}

float UARMMC_MaxShieldGaugeBase::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude( VitDef, Spec, EvaluationParameters, Vitality );

	float ShieldGaugeRate = 0.f;
	GetCapturedAttributeMagnitude( ShieldGaugeRateDef, Spec, EvaluationParameters, ShieldGaugeRate );

	float MaxShieldGaugeBase = Vitality * ShieldGaugeRate;

	return MaxShieldGaugeBase;
}