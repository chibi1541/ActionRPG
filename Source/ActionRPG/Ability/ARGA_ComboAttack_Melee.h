// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_SpendStamina.h"
#include "Interface/ARUsedTraceHitInterface.h"

#include "ARGA_ComboAttack_Melee.generated.h"

USTRUCT( BlueprintType )
struct FAttackComboData
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly )
		int ComboIndex;

	UPROPERTY( EditDefaultsOnly )
		TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY( EditDefaultsOnly )
		int ExtraStaminaCost;

	UPROPERTY( EditDefaultsOnly )
		int ExtraDamage;
};

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGA_ComboAttack_Melee : public UARGA_SpendStamina, public IARUsedTraceHitInterface
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec ) override;

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	virtual void InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) override;

	virtual bool SpendStamina() override;

	UFUNCTION()
		void OnCompleted();

	UFUNCTION()
		void OnCancelled();

	// UsedTraceHit Interface
	virtual void OnHit_Implementation( FHitResult HitResult ) override;

private:
	UFUNCTION()
		void OnPlayMontage( int CurrentIndex );

	UFUNCTION()
		bool NextAttackAvailable() const;

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TArray<FAttackComboData> ComboAttackDatas;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		FGameplayTag NextAttackTag;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect" )
		TSubclassOf<UGameplayEffect> DamageCalcEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect" )
		TSubclassOf<UGameplayEffect> StiffEffect;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Trigger" )
		bool OnlyTriggerOnce;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting" )
		bool StopWhenAbilityEnds;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate" )
		float AnimRootMotionTranslationScale;

	bool bNextAttack;

	int CurComboIndex;
};
