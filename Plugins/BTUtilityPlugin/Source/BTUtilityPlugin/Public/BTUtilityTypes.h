// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define MAX_UTILITY_CHILDREN 16

typedef TArray< uint8, TFixedAllocator< MAX_UTILITY_CHILDREN > > FUtilityExecutionOrdering;
