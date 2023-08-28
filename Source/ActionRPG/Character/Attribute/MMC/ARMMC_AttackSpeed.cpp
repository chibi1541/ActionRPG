// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_AttackSpeed.h"

#include "Character/Attribute/ARBaseAttribSet.h"
#include "Subsystem/Attribute/ARAttributeSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_AttackSpeed)

UARMMC_AttackSpeed::UARMMC_AttackSpeed( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetAgilityAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
}

float UARMMC_AttackSpeed::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Agility = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Agility );

	float AttackSpeedCoeff = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		AttackSpeedCoeff = CoeffData->AttackSpeedCoeff;
	}

	float ModifiedAttackSpeed = 1.0f + ( Agility * AttackSpeedCoeff );

	return ModifiedAttackSpeed;
}