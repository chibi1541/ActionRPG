// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator/BTDecorator_UtilityCurve.h"

#include "Curves/CurveFloat.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_UtilityCurve)

UBTDecorator_UtilityCurve::UBTDecorator_UtilityCurve( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "Utility Curve" );

	StateValueKey.AddFloatFilter( this, GET_MEMBER_NAME_CHECKED( UBTDecorator_UtilityCurve, StateValueKey ) );
	StateValueKey.AddIntFilter( this, GET_MEMBER_NAME_CHECKED( UBTDecorator_UtilityCurve, StateValueKey ) );
}

void UBTDecorator_UtilityCurve::InitializeFromAsset( UBehaviorTree& Asset )
{
	Super::InitializeFromAsset( Asset );

	StateValueKey.ResolveSelectedKey( *GetBlackboardAsset() );
}

FString UBTDecorator_UtilityCurve::GetStaticDescription() const
{
	FString CurveName;
	if( CurveValue != nullptr )
	{
		CurveName = CurveValue->GetName();
	}

	return FString::Printf( TEXT( "Utility Key : %s" ), *CurveName );
}

void UBTDecorator_UtilityCurve::DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const
{
	Super::DescribeRuntimeValues( OwnerComp, NodeMemory, Verbosity, Values );

	FString DescKeyValue;

	if( CurveValue != nullptr )
	{
		DescKeyValue = FString::SanitizeFloat( CurveValue->GetFloatValue( GetStateValue( OwnerComp ) ) );
	}

	Values.Add( FString::Printf( TEXT( "Utility : %s" ), *DescKeyValue ) );
}

float UBTDecorator_UtilityCurve::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	if( CurveValue != nullptr )
	{
		return CurveValue->GetFloatValue( GetStateValue( OwnerComp ) );
	}

	return 0.0f;
}

const float UBTDecorator_UtilityCurve::GetStateValue( const UBehaviorTreeComponent& OwnerComp ) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	float Value = 0.0f;

	if( StateValueKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() )
	{
		Value = MyBlackboard->GetValue<UBlackboardKeyType_Float>( StateValueKey.GetSelectedKeyID() );
	}
	else if( StateValueKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass() )
	{
		Value = ( float )MyBlackboard->GetValue<UBlackboardKeyType_Int>( StateValueKey.GetSelectedKeyID() );
	}

	return FMath::Max( Value, 0.0f );
}
