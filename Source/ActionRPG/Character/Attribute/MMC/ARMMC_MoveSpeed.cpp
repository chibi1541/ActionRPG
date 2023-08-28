// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_MoveSpeed.h"

#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_MoveSpeed)

UARMMC_MoveSpeed::UARMMC_MoveSpeed( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	BaseAttribDef.AttributeToCapture = UARBaseAttribSet::GetAgilityAttribute();
	BaseAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseAttribDef.bSnapshot = false;

	BaseMoveSpeedDef.AttributeToCapture = UARBaseAttribSet::GetBaseMoveSpeedAttribute();
	BaseMoveSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseMoveSpeedDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( BaseAttribDef );
	RelevantAttributesToCapture.Add( BaseMoveSpeedDef );
}

float UARMMC_MoveSpeed::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Agility = 0.f;
	GetCapturedAttributeMagnitude( BaseAttribDef, Spec, EvaluationParameters, Agility );
	Agility = FMath::Max( Agility, 0.f );

	float BaseMoveSpeed = 0.f;
	GetCapturedAttributeMagnitude( BaseMoveSpeedDef, Spec, EvaluationParameters, BaseMoveSpeed );
	BaseMoveSpeed = FMath::Max( BaseMoveSpeed, 0.f );

	float MoveSpeedCoeff = 0.f;
	const FCharacterCoefficientData* CoeffData = GetCoefficientData( Spec );
	if( CoeffData )
	{
		MoveSpeedCoeff = CoeffData->MoveSpeedCoeff;
	}

	float ModifiedMoveSpeed = BaseMoveSpeed + ( Agility * MoveSpeedCoeff );

	return ModifiedMoveSpeed;
}
