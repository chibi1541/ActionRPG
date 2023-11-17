// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/ARBTUtilitySelection_Proportion.h"

template < typename ActiveFilter >
static int32 ProportionalSelect( const TArray< float >& Values, const ActiveFilter& Filter, const float Sum )
{
	auto SelectionValue = FMath::FRandRange( 0.0f, Sum );
	auto Cumulative = 0.0f;
	for( int32 Idx = 0; Idx < Values.Num(); ++Idx )
	{
		if( Filter( Idx ) )
		{
			Cumulative += Values[Idx];
			if( SelectionValue <= Cumulative )
			{
				return Idx;
			}
		}
	}

	check( false );
	return 0;
}

template < typename ActiveFilter >
static int32 ProportionalSelect( const TArray< float >& Values, const ActiveFilter& Filter )
{
	float Sum = 0.0f;
	for( int32 Idx = 0; Idx < Values.Num(); ++Idx )
	{
		if( Filter( Idx ) )
		{
			Sum += Values[Idx];
		}
	}

	return ProportionalSelect( Values, Filter, Sum );
}

namespace UtilitySelection
{
	void ProportionalOrdering( const TArray< float >& UtilityValues, OUT FUtilityExecutionOrdering& ExecutionOrdering )
	{
		const auto Count = UtilityValues.Num();

		// Calculate the sum of all utility values
		float UtilitySum = 0.0f;
		for( auto Ut : UtilityValues )
		{
			UtilitySum += Ut;
		}

		// Create an array denoting which children remain to be chosen
		TArray< bool > Remaining;
		Remaining.Init( true, Count );

		ExecutionOrdering.Reset( Count );
		for( int32 Iteration = 0; Iteration < Count - 1; ++Iteration )
		{
			// Select the next highest priority child, based on the utility values of those remaining
			auto ChildIdx = ProportionalSelect( UtilityValues, [&Remaining]( int32 Idx ) { return Remaining[Idx]; }, UtilitySum );
			ExecutionOrdering.Add( ChildIdx );

			// Reduce the remaining utility sum and remove the child from the remaining array
			UtilitySum -= UtilityValues[ChildIdx];
			UtilitySum = FMath::Max( UtilitySum, 0.0f );
			Remaining[ChildIdx] = false;
		}

		// Fill in the only remaining child index
		ExecutionOrdering.Add( Remaining.IndexOfByKey( true ) );
	}
}
