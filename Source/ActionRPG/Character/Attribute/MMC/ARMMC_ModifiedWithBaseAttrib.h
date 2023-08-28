// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Subsystem/Attribute/ARAttributeSubsystem.h"

#include "ARMMC_ModifiedWithBaseAttrib.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARMMC_ModifiedWithBaseAttrib : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
protected:
	const FCharacterCoefficientData* GetCoefficientData( const FGameplayEffectSpec& Spec ) const;

protected:
	FGameplayEffectAttributeCaptureDefinition BaseAttribDef;
};
