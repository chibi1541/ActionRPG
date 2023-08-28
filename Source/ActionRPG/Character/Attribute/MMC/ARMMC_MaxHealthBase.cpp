// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_MaxHealthBase.h"

#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_MaxHealthBase)

UARMMC_MaxHealthBase::UARMMC_MaxHealthBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetVitalityAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
}

float UARMMC_MaxHealthBase::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Vitality );
	Vitality = FMath::Max( Vitality, 0.f );

	float HealthCoeff = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		HealthCoeff = CoeffData->HealthCoeff;
	}

	float MaxHealthBase = Vitality * HealthCoeff;

	return MaxHealthBase;
}
