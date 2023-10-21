// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_TriggerOtherAbility.h"

#include "ARGA_Casting.generated.h"


class UAnimMontage;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARGA_Casting : public UARGA_TriggerOtherAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	UFUNCTION()
		void OnCompleted();

	UFUNCTION()
		void OnCancelled();

	virtual void TriggerAbility() override;


protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		TObjectPtr<UAnimMontage> CastingMontage;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		float CastingTime;

	FTimerHandle Timer;

};
