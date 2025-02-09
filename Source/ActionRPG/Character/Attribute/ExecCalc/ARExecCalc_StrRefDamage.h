// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameplayEffectExecutionCalculation.h"

#include "ARExecCalc_StrRefDamage.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARExecCalc_StrRefDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const override;
};
