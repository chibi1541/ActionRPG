// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/ARBTComposite_Utility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARBTComposite_Utility)


UARBTComposite_Utility::UARBTComposite_Utility( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "Utility" );
	bUseNodeActivationNotify = true;

	SelectionMethodType = EUtilitySelectionMethodType::Priority;
}

void UARBTComposite_Utility::InitializeMemory( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType ) const
{
	FARBTUtilityMemory* UtilityMemory = CastInstanceNodeMemory< FARBTUtilityMemory >( NodeMemory );
	if( InitType == EBTMemoryInit::Initialize )
	{
		new ( &UtilityMemory->ExecutionOrdering ) FUtilityExecutionOrdering();
	}
}

uint16 UARBTComposite_Utility::GetInstanceMemorySize() const
{
	return sizeof( FARBTUtilityMemory );
}

FString UARBTComposite_Utility::GetStaticDescription() const
{
	switch( SelectionMethodType )
	{
	case EUtilitySelectionMethodType::Priority:
	return TEXT( "Priority selection" );

	case EUtilitySelectionMethodType::Proportional:
	return TEXT( "Proportional selection" );

	default:
	check( false );
	return{};
	}
}

void UARBTComposite_Utility::NotifyNodeActivation( FBehaviorTreeSearchData& SearchData ) const
{
	FARBTUtilityMemory* NodeMemory = GetNodeMemory<FARBTUtilityMemory>( SearchData );

	// Evaluate utility scores for each child
	TArray< float > UtilityValues;
	//bool bNonZeroUtility = EvaluateUtilityScores( SearchData, UtilityValues );

	// Generate ordering
	switch( SelectionMethodType )
	{
	case EUtilitySelectionMethodType::Priority:
	//UtilitySelection::PriorityOrdering( UtilityValues, NodeMemory->ExecutionOrdering );
	break;
	case EUtilitySelectionMethodType::Proportional:
	//UtilitySelection::ProportionalOrdering( UtilityValues, NodeMemory->ExecutionOrdering );
	break;
	default:
	check( false );
	}
}

//const UBTDecorator_UtilityFunction* UARBTComposite_Utility::FindChildUtilityFunction( int32 ChildIndex ) const
//{
//	auto const& ChildInfo = Children[ChildIndex];
//	for( auto Dec : ChildInfo.Decorators )
//	{
//		auto AsUtilFunc = Cast< UBTDecorator_UtilityFunction >( Dec );
//		if( AsUtilFunc )
//		{
//			// Take the first one. Multiple utility function decorators on a single node is a user
//			// error, and generates a warning in the behavior tree editor.
//			return AsUtilFunc;
//		}
//	}
//
//	// Child does not have a utility function decorator
//	return nullptr;
//}

