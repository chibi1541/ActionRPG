// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGameplayAbility.h"

#include "ARGA_SpendStamina.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGA_SpendStamina : public UARGameplayAbility
{
	GENERATED_UCLASS_BODY()


protected:
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	virtual bool SpendStamina();

	virtual const bool IsExhausted() const;

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CostStamina|Effect" )
		TSubclassOf<UGameplayEffect> StaminaSpandEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CostStamina|Value", meta = ( ClampMax = "0" ) )
		int StaminaCost;

	FGameplayTag ExhaustedTag;

	UPROPERTY()
		TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
