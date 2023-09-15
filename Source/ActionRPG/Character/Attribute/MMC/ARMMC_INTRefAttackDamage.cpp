// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_INTRefAttackDamage.h"

#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_INTRefAttackDamage)

UARMMC_INTRefAttackDamage::UARMMC_INTRefAttackDamage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetIntelligenceAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
}

float UARMMC_INTRefAttackDamage::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;
	EvaluationParameters.SourceTags = SourceTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Intelligence );
	Intelligence = FMath::Max( Intelligence, 0.f );

	float DamageCoefficient = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		DamageCoefficient = CoeffData->AttackDamageCoeff;
	}

	float AttackDamage = Intelligence * DamageCoefficient;

	return AttackDamage;
}
