// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"

#include "ARBTDecorator_UtilityFunction.generated.h"

/**
 *
 */
UCLASS( Abstract )
class ACTIONRPG_API UARBTDecorator_UtilityFunction : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

public:
	float WrappedCalculateUtility( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const;

protected:
	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const;


};
