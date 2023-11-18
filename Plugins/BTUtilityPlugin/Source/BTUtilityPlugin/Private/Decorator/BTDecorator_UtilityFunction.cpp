// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator/BTDecorator_UtilityFunction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_UtilityFunction)

UBTDecorator_UtilityFunction::UBTDecorator_UtilityFunction( const FObjectInitializer& ObjectInitialier )
	:Super( ObjectInitialier )
{
	NodeName = TEXT( "UnknownUtilityFunction" );

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

float UBTDecorator_UtilityFunction::WrappedCalculateUtility( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	const UBTDecorator_UtilityFunction* NodeObj = bCreateNodeInstance ? ( const UBTDecorator_UtilityFunction* )GetNodeInstance( OwnerComp, NodeMemory ) : this;
	return NodeObj ? NodeObj->CalculateUtilityValue( OwnerComp, NodeMemory ) : 0.0f;
}


float UBTDecorator_UtilityFunction::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	return 0.0f;
}

