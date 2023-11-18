// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Decorator/BTDecorator_UtilityFunction.h"

#include "BTDecorator_UtilityBlackboard.generated.h"

class UBehaviorTree;

/**
 * 
 */

UCLASS( Meta = ( DisplayName = "Utility Blackboard", Category = "Utility Functions" ) )
class BTUTILITYPLUGIN_API UBTDecorator_UtilityBlackboard : public UBTDecorator_UtilityFunction
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeFromAsset( UBehaviorTree& Asset ) override;

	virtual FString GetStaticDescription() const override;
	virtual void DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const override;

protected:
	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const override;

protected:
	UPROPERTY( EditAnywhere, Category = "Blackboard" )
		struct FBlackboardKeySelector UtilityValueKey;

private:
	FORCEINLINE FName GetSelectedBlackboardKey() const { return UtilityValueKey.SelectedKeyName; }

};
