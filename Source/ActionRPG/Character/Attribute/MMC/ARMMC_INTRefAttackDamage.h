// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attribute/MMC/ARMMC_ModifiedWithBaseAttrib.h"
#include "ARMMC_INTRefAttackDamage.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARMMC_INTRefAttackDamage : public UARMMC_ModifiedWithBaseAttrib
{
	GENERATED_UCLASS_BODY()

protected:
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;

};
