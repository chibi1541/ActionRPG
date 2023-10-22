// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM( BlueprintType )
enum class EGameplayEffectDelegateType :uint8
{
	EDT_None		UMETA( DisplayName = "None" ),
	EDT_Stiff		UMETA( DisplayName = "Stiff" ),
	EDT_Stun		UMETA( DisplayName = "Stun" ),
	EDT_Provoke		UMETA( DisplayName = "Provoke" ),
	EDT_Guard		UMETA( DisplayName = "Guard" ),
};