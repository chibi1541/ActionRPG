// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/ARBTDecorator_UtilityFunction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARBTDecorator_UtilityFunction)


UARBTDecorator_UtilityFunction::UARBTDecorator_UtilityFunction( const FObjectInitializer& ObjectInitialier )
	:Super( ObjectInitialier )
{
	NodeName = TEXT( "UnknownUtilityFunction" );

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

float UARBTDecorator_UtilityFunction::WrappedCalculateUtility( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	const UARBTDecorator_UtilityFunction* NodeObj = bCreateNodeInstance ? ( const UARBTDecorator_UtilityFunction* )GetNodeInstance( OwnerComp, NodeMemory ) : this;
	return NodeObj ? NodeObj->CalculateUtilityValue( OwnerComp, NodeMemory ) : 0.0f;
}


float UARBTDecorator_UtilityFunction::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	return 0.0f;
}


