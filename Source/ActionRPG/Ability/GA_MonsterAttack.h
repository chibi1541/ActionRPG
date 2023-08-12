// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Ability/ARPGGameplayAbility.h"

#include "GA_MonsterAttack.generated.h"

class UAmimMontage;
class UHitTraceComponent;
struct FGameplayTag;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UGA_MonsterAttack : public UARPGGameplayAbility
{
	GENERATED_UCLASS_BODY()

private:
	virtual void OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec ) override;

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	UFUNCTION()
		void OnCancelled();

	UFUNCTION()
		void OnCompleted();

private:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Attack|AttackTrace", meta = ( AllowPrivateAccess = "true" ) )
		TObjectPtr<UHitTraceComponent> HitTraceComp;

	UPROPERTY( VisibleAnywhere, Category = "AnimMontage|Montages", meta = ( AllowPrivateAccess = "true" ) )
		TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
		float Rate;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting", meta = ( AllowPrivateAccess = "true" ) )
		bool StopWhenAbilityEnd;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
		float AnimRootMotionTranslationScale;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StartTime", meta = ( AllowPrivateAccess = "true" ) )
		float StartTimeSec;
};
