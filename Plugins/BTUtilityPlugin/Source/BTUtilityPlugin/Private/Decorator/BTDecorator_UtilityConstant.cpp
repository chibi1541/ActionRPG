// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator/BTDecorator_UtilityConstant.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_UtilityConstant)

UBTDecorator_UtilityConstant::UBTDecorator_UtilityConstant( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "Utility Constant" );
	UtilityValue = 0.f;
}

FString UBTDecorator_UtilityConstant::GetStaticDescription() const
{
	return FString::Printf( TEXT( "Utility: %.1f" ), UtilityValue );
}

void UBTDecorator_UtilityConstant::DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const
{
	Super::DescribeRuntimeValues( OwnerComp, NodeMemory, Verbosity, Values );
}

float UBTDecorator_UtilityConstant::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	return UtilityValue;
}
