// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Attribute/MMC/ARMMC_ModifiedWithBaseAttrib.h"

#include "ARMMC_MoveSpeed.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARMMC_MoveSpeed : public UARMMC_ModifiedWithBaseAttrib
{
	GENERATED_UCLASS_BODY()


protected:
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;

private:
	FGameplayEffectAttributeCaptureDefinition BaseMoveSpeedDef;

};
