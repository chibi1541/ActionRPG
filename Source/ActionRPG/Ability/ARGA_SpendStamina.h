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

	virtual void EndAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled ) override;

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CostStamina|Effect" )
		TSubclassOf<UGameplayEffect> StaminaSpandEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CostStamina|Effect" )
		TSubclassOf<UGameplayEffect> StaminaRegenDelayEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CostStamina|Value", meta = ( ClampMax = "0" ) )
		int StaminaCost;

	FGameplayTag ExhaustedTag;
	FGameplayTag StaminaSpendTag;

	UPROPERTY()
		TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
