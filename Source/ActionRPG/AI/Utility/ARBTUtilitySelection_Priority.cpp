// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/ARBTUtilitySelection_Priority.h"

namespace UtilitySelection
{
	void PriorityOrdering( const TArray< float >& UtilityValues, OUT FUtilityExecutionOrdering& ExecutionOrdering )
	{
		struct FIndexedUtilityValue
		{
			int32 ChildIdx;
			float Value;

			inline bool operator< ( const FIndexedUtilityValue& Other ) const
			{
				return Value > Other.Value;
			}
		};

		// Index the values and sort them
		const auto Count = UtilityValues.Num();
		TArray< FIndexedUtilityValue > IndexedValues;
		IndexedValues.SetNumUninitialized( Count );
		for( int32 ChildIdx = 0; ChildIdx < Count; ++ChildIdx )
		{
			IndexedValues[ChildIdx].ChildIdx = ChildIdx;
			IndexedValues[ChildIdx].Value = UtilityValues[ChildIdx];
		}

		IndexedValues.Sort();

		// Output the sorted indexes
		ExecutionOrdering.SetNumUninitialized( Count );
		for( int32 ExIdx = 0; ExIdx < Count; ++ExIdx )
		{
			ExecutionOrdering[ExIdx] = IndexedValues[ExIdx].ChildIdx;
		}
	}
}