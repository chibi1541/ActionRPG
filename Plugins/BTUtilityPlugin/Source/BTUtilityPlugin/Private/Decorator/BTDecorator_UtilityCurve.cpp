// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator/BTDecorator_UtilityCurve.h"

#include "Curves/CurveFloat.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_UtilityCurve)

UBTDecorator_UtilityCurve::UBTDecorator_UtilityCurve( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "Utility Curve" );

}

void UBTDecorator_UtilityCurve::InitializeFromAsset( UBehaviorTree& Asset )
{
	Super::InitializeFromAsset( Asset );

	for( auto CurveInfo : CurveInfoArray )
	{
		CurveInfo.StateValueKey.ResolveSelectedKey( *GetBlackboardAsset() );
	}
}

FString UBTDecorator_UtilityCurve::GetStaticDescription() const
{
	FString CurveName;
	if( !CurveInfoArray.IsEmpty() )
	{
		for( const auto CurveInfo : CurveInfoArray )
		{
			if( CurveInfo.CurveValue )
			{
				CurveName.Append( FString::Printf( TEXT( "%s, " ), *CurveInfo.CurveValue->GetName() ) );
			}
			else
			{
				CurveName.Append( TEXT( "None, " ) );
			}
		}

	}

	return FString::Printf( TEXT( "Utility Key : %s" ), *CurveName );
}

void UBTDecorator_UtilityCurve::DescribeRuntimeValues( const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values ) const
{
	Super::DescribeRuntimeValues( OwnerComp, NodeMemory, Verbosity, Values );

	FString DescKeyValue;
	if( !CurveInfoArray.IsEmpty() )
	{
		for( int Index = 0; Index < CurveInfoArray.Num(); Index++ )
		{
			if( CurveInfoArray[Index].CurveValue )
			{
				DescKeyValue.Append(
					FString::Printf( TEXT( "%s, " ), *FString::SanitizeFloat(
					CurveInfoArray[Index].CurveValue->GetFloatValue(
					GetStateValue( OwnerComp, Index ) ) ) ) );
			}
			else
			{
				DescKeyValue.Append( TEXT( "None, " ) );
			}
		}
	}

	Values.Add( FString::Printf( TEXT( "Utility : %s" ), *DescKeyValue ) );
}

float UBTDecorator_UtilityCurve::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	float CurveValueSum = 0.f;

	if( !CurveInfoArray.IsEmpty() )
	{
		for( int Index = 0; Index < CurveInfoArray.Num(); Index++ )
		{
			if( !CurveInfoArray[Index].CurveValue )
			{
				// error log
				continue;
			}
			else
			{
				CurveValueSum += CurveInfoArray[Index].CurveValue->GetFloatValue( GetStateValue( OwnerComp, Index ) );
			}
		}
	}

	return ( CurveInfoArray.Num() > 0 ) ? CurveValueSum / CurveInfoArray.Num() : 0.f;
}

const float UBTDecorator_UtilityCurve::GetStateValue( const UBehaviorTreeComponent& OwnerComp, const int& ArrayIndex ) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	float Value = 0.0f;

	const auto Curve = CurveInfoArray[ArrayIndex];

	if( Curve.StateValueKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() )
	{
		Value = MyBlackboard->GetValue<UBlackboardKeyType_Float>( Curve.StateValueKey.GetSelectedKeyID() );
	}
	else if( Curve.StateValueKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass() )
	{
		Value = ( float )MyBlackboard->GetValue<UBlackboardKeyType_Int>( Curve.StateValueKey.GetSelectedKeyID() );
	}
	else
	{
		// show error log
	}

	return FMath::Max( Value, 0.0f );
}
