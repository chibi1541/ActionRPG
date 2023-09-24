// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_HealthRegene.h"

#include "Character/Attribute/ARVitRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_HealthRegene)

UARMMC_HealthRegene::UARMMC_HealthRegene( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	MaxHealthAttribDef.AttributeToCapture = UARVitRefAttribSet::GetMaxHealthAttribute();
	MaxHealthAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxHealthAttribDef.bSnapshot = false;

	HealthRegeneAttribDef.AttributeToCapture = UARVitRefAttribSet::GetHealthRegeneAttribute();
	HealthRegeneAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HealthRegeneAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( MaxHealthAttribDef );
	RelevantAttributesToCapture.Add( HealthRegeneAttribDef );
}

float UARMMC_HealthRegene::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude( MaxHealthAttribDef, Spec, EvaluationParameters, MaxHealth );
	MaxHealth = FMath::Max( MaxHealth, 0.f );

	float HealthRegene = 0.f;
	GetCapturedAttributeMagnitude( HealthRegeneAttribDef, Spec, EvaluationParameters, HealthRegene );
	HealthRegene = FMath::Max( HealthRegene, 0.f );

	float RegenPoint = MaxHealth * HealthRegene;

	return RegenPoint;
}
