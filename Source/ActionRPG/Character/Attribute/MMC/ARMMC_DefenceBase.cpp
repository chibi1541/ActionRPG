// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_DefenceBase.h"

#include "Character/Attribute/ARPGBaseAttributeSet.h"
#include "Character/Attribute/ARPGVITAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_DefenceBase)

UARMMC_DefenceBase::UARMMC_DefenceBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	VitDef.AttributeToCapture = UARPGBaseAttributeSet::GetVitalityAttribute();
	VitDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitDef.bSnapshot = false;

	DefenceRateDef.AttributeToCapture = UARPGVITAttributeSet::GetDefenceRateAttribute();
	DefenceRateDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefenceRateDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( VitDef );
	RelevantAttributesToCapture.Add( DefenceRateDef );
}

float UARMMC_DefenceBase::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude( VitDef, Spec, EvaluationParameters, Vitality );

	float DefenceRate = 0.f;
	GetCapturedAttributeMagnitude( DefenceRateDef, Spec, EvaluationParameters, DefenceRate );

	float DefenceBase = Vitality * DefenceRate;

	return DefenceBase;
}

