// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_StaminaRegene.h"

#include "Character/Attribute/ARVitRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_StaminaRegene)

UARMMC_StaminaRegene::UARMMC_StaminaRegene( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	MaxStaminaAttribDef.AttributeToCapture = UARVitRefAttribSet::GetMaxStaminaAttribute();
	MaxStaminaAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxStaminaAttribDef.bSnapshot = false;

	StaminaRegeneAttribDef.AttributeToCapture = UARVitRefAttribSet::GetStaminaRegeneAttribute();
	StaminaRegeneAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	StaminaRegeneAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( MaxStaminaAttribDef );
	RelevantAttributesToCapture.Add( StaminaRegeneAttribDef );
}

float UARMMC_StaminaRegene::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxStamina = 0.f;
	GetCapturedAttributeMagnitude( MaxStaminaAttribDef, Spec, EvaluationParameters, MaxStamina );
	MaxStamina = FMath::Max( MaxStamina, 0.f );

	float StaminaRegene = 0.f;
	GetCapturedAttributeMagnitude( StaminaRegeneAttribDef, Spec, EvaluationParameters, StaminaRegene );
	StaminaRegene = FMath::Max( StaminaRegene, 0.f );

	float RegenPoint = MaxStamina * StaminaRegene;

	return RegenPoint;
}

