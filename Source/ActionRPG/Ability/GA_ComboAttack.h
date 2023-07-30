// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Ability/ARPGGameplayAbility.h"

#include "GA_ComboAttack.generated.h"

class UAmimMontage;
struct FGameplayTag;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UGA_ComboAttack : public UARPGGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ComboAttack( const FObjectInitializer& ObjectInitializer );

protected:
	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	virtual void InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) override;

	UFUNCTION()
	void OnCancelled();

	UFUNCTION()
	void OnCompleted();

private:
	UFUNCTION()
	bool NextAttackAvailable() const;

	UFUNCTION()
	void SetCurComboIndex( int CurrentIndex );

protected:
	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Montages", meta = ( AllowPrivateAccess = "true" ) )
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|EventTag", meta = ( AllowPrivateAccess = "true" ) )
	FGameplayTag EventTag;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
	float Rate;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Trigger", meta = ( AllowPrivateAccess = "true" ) )
	bool OnlyTriggerOnce;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting", meta = ( AllowPrivateAccess = "true" ) )
	bool StopWhenAbilityEnds;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate", meta = ( AllowPrivateAccess = "true" ) )
	float AnimRootMotionTranslationScale;

	UPROPERTY( EditDefaultsOnly, Category = "ComboState", meta = ( AllowPrivateAccess = "true" ) )
	int MaxComboIndex;

	bool bNextAttack;

	int CurComboIndex;


};
