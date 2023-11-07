// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "UObject/ObjectMacros.h"

#include "AREnvQueryContext_Target.generated.h"

struct FEnvQueryContextData;
struct FEnvQueryInstance;

/**
 *
 * Querier Character가 현재 타겟으로 하는 Hero or Monster를 기준으로
 * Gernerator's Center Property
 */
UCLASS( MinimalAPI )
class UAREnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_UCLASS_BODY()

private:
	virtual void ProvideContext( FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData ) const override;

};
