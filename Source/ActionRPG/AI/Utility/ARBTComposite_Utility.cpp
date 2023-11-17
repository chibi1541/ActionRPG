// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/ARBTComposite_Utility.h"

#include "AI/Utility/ARBTDecorator_UtilityFunction.h"
#include "AI/Utility/ARBTUtilitySelection_Priority.h"
#include "AI/Utility/ARBTUtilitySelection_Proportion.h"


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

int32 UARBTComposite_Utility::GetNextChildHandler( FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult ) const
{
	FARBTUtilityMemory* NodeMemory = GetNodeMemory<FARBTUtilityMemory>( SearchData );

	// success = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	check( NodeMemory->ExecutionOrdering.Num() == GetChildrenNum() );
	if( PrevChild == BTSpecialChild::NotInitialized )
	{
		// newly activated: start from first in the ordering
		NextChildIdx = NodeMemory->ExecutionOrdering[0];
	}
	else if( LastResult == EBTNodeResult::Failed )
	{
		// @NOTE: Linear search to find position in ordering of last executed child. This could be avoided but overhead is negligible
		// so seems better to avoid storing extra state in the node memory.
		int32 OrderingIndex = NodeMemory->ExecutionOrdering.IndexOfByKey( PrevChild ) + 1;
		if( OrderingIndex < NodeMemory->ExecutionOrdering.Num() )
		{
			// failed = choose next child in the ordering
			NextChildIdx = NodeMemory->ExecutionOrdering[OrderingIndex];
		}
	}

	return NextChildIdx;
}

const UARBTDecorator_UtilityFunction* UARBTComposite_Utility::FindChildUtilityFunction( int32 ChildIndex ) const
{
	const auto& ChildInfo = Children[ChildIndex];
	for( auto Deco : ChildInfo.Decorators )
	{
		auto UtilityFunc = Cast< UARBTDecorator_UtilityFunction >( Deco );
		if( UtilityFunc )
		{
			return UtilityFunc;
		}
	}

	// Child does not have a utility function decorator
	return nullptr;
}


void UARBTComposite_Utility::NotifyNodeActivation( FBehaviorTreeSearchData& SearchData ) const
{
	FARBTUtilityMemory* NodeMemory = GetNodeMemory<FARBTUtilityMemory>( SearchData );

	TArray< float > UtilityValues;
	bool bNonZeroUtility = EvaluateUtilityScores( SearchData, UtilityValues );

	switch( SelectionMethodType )
	{
	case EUtilitySelectionMethodType::Priority:
		UtilitySelection::PriorityOrdering( UtilityValues, NodeMemory->ExecutionOrdering );
		break;

	case EUtilitySelectionMethodType::Proportional:
		UtilitySelection::ProportionalOrdering( UtilityValues, NodeMemory->ExecutionOrdering );
		break;

	default:
		check( false );

	}
}

bool UARBTComposite_Utility::EvaluateUtilityScores( FBehaviorTreeSearchData& SearchData, OUT TArray<float>& Scores ) const
{
	bool bIsNonZeroScore = false;
	for( int32 Idx = 0; Idx < GetChildrenNum(); ++Idx )
	{
		auto UtilityFunc = FindChildUtilityFunction( Idx );

		auto Score = UtilityFunc ?
			UtilityFunc->WrappedCalculateUtility( SearchData.OwnerComp, UtilityFunc->GetNodeMemory< uint8 >( SearchData ) ) :
			0.0f;

		Scores.Add( Score );
		bIsNonZeroScore = bIsNonZeroScore || Score > 0.0f;
	}

	return bIsNonZeroScore;

}


