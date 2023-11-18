// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_UtilityFunction.generated.h"

/**
 *
 */
UCLASS( Abstract )
class BTUTILITYPLUGIN_API UBTDecorator_UtilityFunction : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

public:
	float WrappedCalculateUtility( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const;

protected:
	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const;


};
