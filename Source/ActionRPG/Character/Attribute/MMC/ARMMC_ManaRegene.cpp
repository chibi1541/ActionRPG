// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_ManaRegene.h"

#include "Character/Attribute/ARIntRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_ManaRegene)

UARMMC_ManaRegene::UARMMC_ManaRegene( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	MaxManaAttribDef.AttributeToCapture = UARIntRefAttribSet::GetMaxManaAttribute();
	MaxManaAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxManaAttribDef.bSnapshot = false;

	ManaRegeneAttribDef.AttributeToCapture = UARIntRefAttribSet::GetManaRegeneAttribute();
	ManaRegeneAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ManaRegeneAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( MaxManaAttribDef );
	RelevantAttributesToCapture.Add( ManaRegeneAttribDef );
}

float UARMMC_ManaRegene::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxMana = 0.f;
	GetCapturedAttributeMagnitude( MaxManaAttribDef, Spec, EvaluationParameters, MaxMana );
	MaxMana = FMath::Max( MaxMana, 0.f );

	float ManaRegene = 0.f;
	GetCapturedAttributeMagnitude( ManaRegeneAttribDef, Spec, EvaluationParameters, ManaRegene );
	ManaRegene = FMath::Max( ManaRegene, 0.f );

	float RegenPoint = MaxMana * ManaRegene;

	return RegenPoint;
}
