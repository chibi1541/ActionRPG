// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_HealHp_Percentage.h"

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_HealHp_Percentage)


UARMMC_HealHp_Percentage::UARMMC_HealHp_Percentage( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	MaxHealthAttribDef.AttributeToCapture = UARVitRefAttribSet::GetMaxHealthAttribute();
	MaxHealthAttribDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthAttribDef.bSnapshot = false;

	RelevantAttributesToCapture.Add( MaxHealthAttribDef );
}

float UARMMC_HealHp_Percentage::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude( MaxHealthAttribDef, Spec, EvaluationParameters, MaxHealth );
	MaxHealth = FMath::Max( MaxHealth, 0.f );

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	float Percentage = Spec.GetSetByCallerMagnitude( Tags.PercentageTag, false, 0.f );

	Percentage = FMath::Clamp( Percentage, 0.f, 1.f );

	float HealPoint = MaxHealth * Percentage;
	return HealPoint;
}
