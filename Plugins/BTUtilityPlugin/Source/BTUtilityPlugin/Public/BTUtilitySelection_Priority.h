// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BTUtilityTypes.h"

namespace UtilitySelection
{
	void PriorityOrdering( const TArray< float >& UtilityValues, OUT FUtilityExecutionOrdering& ExecutionOrdering );
}