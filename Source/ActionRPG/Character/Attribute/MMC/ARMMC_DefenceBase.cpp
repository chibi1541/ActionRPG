// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_DefenceBase.h"

#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_DefenceBase)

UARMMC_DefenceBase::UARMMC_DefenceBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetVitalityAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
}

float UARMMC_DefenceBase::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Vitality );
	Vitality = FMath::Max( Vitality, 0.f );

	float DefenceCoeff = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		DefenceCoeff = CoeffData->DefenceCoeff;
	}

	float DefenceBase = Vitality * DefenceCoeff;

	return DefenceBase;
}

