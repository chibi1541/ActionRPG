// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Utility/ARBTUtilityTypes.h"

namespace UtilitySelection
{
	void ProportionalOrdering( const TArray< float >& UtilityValues, OUT FUtilityExecutionOrdering& ExecutionOrdering );
}
