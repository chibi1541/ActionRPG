// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_UsingCooldown.h"

#include "ARGA_Blink.generated.h"

/**
 *
 */
 class UNavigationSystemV1;

UCLASS()
class ACTIONRPG_API UARGA_Blink : public UARGA_UsingCooldown
{
	GENERATED_UCLASS_BODY()

protected:

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

private:
	UFUNCTION()
		void OnPlayMontage( int CurrentIndex );

	UFUNCTION()
		void OnCompleted();

	UFUNCTION()
		void OnCancelled();

	virtual void EndAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled ) override;

protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		TArray<TObjectPtr<UAnimMontage>> MontageList;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect" )
		TSubclassOf<UGameplayEffect> ImmuneEffect;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting" )
		bool StopWhenAbilityEnds;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|RootMotion" )
		float AnimRootMotionTranslationScale;

	UPROPERTY( EditDefaultsOnly, meta = ( ClampMin = 0 ) )
		float Range;
	
	FActiveGameplayEffectHandle MovingLockEffectHandle;

	FVector ForwardVector;
};
