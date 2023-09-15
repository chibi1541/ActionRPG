// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_SpendStamina.h"
#include "Interface/ARUsedTraceHitInterface.h"

#include "ARGA_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARGA_MeleeAttack : public UARGA_SpendStamina, public IARUsedTraceHitInterface
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec ) override;

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	UFUNCTION()
		void OnCancelled();

	UFUNCTION()
		void OnCompleted();

	virtual void OnHit_Implementation( FHitResult HitResult ) override;

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AnimMontage|Montages", meta = ( AllowPrivateAccess = "true" ) )
		TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
		float Rate;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting", meta = ( AllowPrivateAccess = "true" ) )
		bool StopWhenAbilityEnd;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
		float AnimRootMotionTranslationScale;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StartTime", meta = ( AllowPrivateAccess = "true" ) )
		float StartTimeSec;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = ( AllowPrivateAccess = "true" ) )
		TSubclassOf<UGameplayEffect> DamageCalcEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = ( AllowPrivateAccess = "true" ) )
		TSubclassOf<UGameplayEffect> StiffEffect;
};
