// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameplayModMagnitudeCalculation.h"

#include "ARMMC_HealthRegene.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARMMC_HealthRegene : public UGameplayModMagnitudeCalculation
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition MaxHealthAttribDef;

	FGameplayEffectAttributeCaptureDefinition HealthRegeneAttribDef;
};
