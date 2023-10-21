// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGameplayAbility.h"

#include "ARGA_TriggerOtherAbility.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGA_TriggerOtherAbility : public UARGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION( BlueprintNativeEvent )
	void MakePayloadData(FGameplayEventData& Payload);

	UFUNCTION()
	virtual void TriggerAbility();

protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		FGameplayTag TriggerAbilityTag;



};
