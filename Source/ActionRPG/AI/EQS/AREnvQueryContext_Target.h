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
 * Querier Character�� ���� Ÿ������ �ϴ� Hero or Monster�� ��������
 * Gernerator's Center Property
 */
UCLASS( MinimalAPI )
class UAREnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_UCLASS_BODY()

private:
	virtual void ProvideContext( FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData ) const override;

};
