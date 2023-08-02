// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Ability/ARPGGameplayAbility.h"

#include "GA_ComboAttack.generated.h"

class UAmimMontage;
class UHitTraceComponent;
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
	// virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|AttackTrace" )
	TObjectPtr<UHitTraceComponent> HitTraceComp;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Montages" )
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|EventTag" )
	FGameplayTag EventTag;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate" )
	float Rate;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Trigger" )
	bool OnlyTriggerOnce;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting" )
	bool StopWhenAbilityEnds;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate" )
	float AnimRootMotionTranslationScale;

	UPROPERTY( EditDefaultsOnly, Category = "ComboState" )
	int MaxComboIndex;

	bool bNextAttack;

	int CurComboIndex;


};
