// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_SpendStaminaOnSprint.h"

#include "Ability/ActionRPGGlobalTags.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/Controller.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_SpendStaminaOnSprint)

UARMMC_SpendStaminaOnSprint::UARMMC_SpendStaminaOnSprint( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

float UARMMC_SpendStaminaOnSprint::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float SpendPoint = 0.f;
	const auto Character = Cast<ABaseCharacter>( Spec.GetContext().GetEffectCauser() );
	if( Character )
	{
		FVector SpeedVector = Character->GetCurrentVelocity();
		float MoveSpeed = SpeedVector.Size2D();
		if( MoveSpeed > 0.f )
		{
			const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
			SpendPoint = Spec.GetSetByCallerMagnitude( Tags.AbilityCostTag_Stamina, false, 3.0f );
			SpendPoint *= Spec.Period;
			SpendPoint *= -1.f;
		}
	}

	return SpendPoint;
}
