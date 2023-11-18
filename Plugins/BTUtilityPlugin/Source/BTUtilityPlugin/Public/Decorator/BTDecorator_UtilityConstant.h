// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Decorator/BTDecorator_UtilityFunction.h"

#include "BTDecorator_UtilityConstant.generated.h"

/**
 * 
 */
UCLASS( Meta = ( DisplayName = "Utility Constant", Category = "Utility Functions" ) )
class BTUTILITYPLUGIN_API UBTDecorator_UtilityConstant : public UBTDecorator_UtilityFunction
{
	GENERATED_UCLASS_BODY()

	virtual FString GetStaticDescription() const override;
	virtual void DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const override;

protected:

	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const override;

public:

	UPROPERTY( EditAnywhere, Category = "Utility" )
		float UtilityValue;

};
