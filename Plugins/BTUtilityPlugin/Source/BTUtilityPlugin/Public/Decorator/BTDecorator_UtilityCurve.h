// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Decorator/BTDecorator_UtilityFunction.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BTDecorator_UtilityCurve.generated.h"

USTRUCT( BlueprintType )
struct BTUTILITYPLUGIN_API FCurveInfo
{
	GENERATED_USTRUCT_BODY()

	FCurveInfo()
	{
	}

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		TObjectPtr<class UCurveFloat> CurveValue;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		FBlackboardKeySelector StateValueKey;
};

/**
 *
 */
UCLASS( Meta = ( DisplayName = "Utility Curve" ), Category = "Utility Functions" )
class BTUTILITYPLUGIN_API UBTDecorator_UtilityCurve : public UBTDecorator_UtilityFunction
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeFromAsset( UBehaviorTree& Asset ) override;

	virtual FString GetStaticDescription() const override;
	virtual void DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const override;

protected:
	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const override;

private:
	const float GetStateValue( const UBehaviorTreeComponent& OwnerComp, const int& ArrayIndex ) const;

protected:
	UPROPERTY( EditAnywhere, Category = "Curve" )
		TArray<FCurveInfo> CurveInfoArray;

};
