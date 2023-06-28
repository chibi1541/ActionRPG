// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Abilities/GameplayAbility.h"

#include "ARPGGameplayAbility.generated.h"

class UAbilitySystemComponent;
struct FGameplayTagContainer;

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

protected:
	virtual bool DoesAbilitySatisfyTagRequirements( const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags , const FGameplayTagContainer* TargetTags , OUT FGameplayTagContainer* OptionalRelevantTags ) const override;

};
