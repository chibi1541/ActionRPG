// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_STRRefAttackDamage.h"

#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_STRRefAttackDamage)

UARMMC_STRRefAttackDamage::UARMMC_STRRefAttackDamage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetStrengthAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
}

float UARMMC_STRRefAttackDamage::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.TargetTags = TargetTags;
	EvaluationParameters.SourceTags = SourceTags;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Strength );
	Strength = FMath::Max( Strength, 0.f );

	float DamageCoefficient = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		DamageCoefficient = CoeffData->AttackDamageCoeff;
	}

	float AttackDamage = Strength * DamageCoefficient;

	return AttackDamage;
}
