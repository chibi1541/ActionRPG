// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_SpendStamina.h"

#include "ARGA_ComboAttackWithProjectile.generated.h"

class AARProjectile;

USTRUCT( BlueprintType )
struct FAttackComboData_Projectile
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly )
		int ComboIndex;

	UPROPERTY( EditDefaultsOnly )
		TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY( EditDefaultsOnly, meta = ( ClampMax = "0" ) )
		int ExtraStaminaCost;

	UPROPERTY( EditDefaultsOnly )
		int ExtraDamage;

	UPROPERTY( EditDefaultsOnly )
		FName StartSocketName;

};

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARGA_ComboAttackWithProjectile : public UARGA_SpendStamina
{
	GENERATED_UCLASS_BODY()
protected:
	virtual void OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec ) override;

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;

	UFUNCTION()
		void OnCompleted();

	UFUNCTION()
		void OnCancelled();


	virtual void InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) override;

	virtual bool SpendStamina() override;

	UFUNCTION()
	virtual void ProjectileFire( FGameplayEventData Payload );

private:
	UFUNCTION()
		void OnPlayMontage( int CurrentIndex );

	UFUNCTION()
		bool NextAttackAvailable() const;

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TArray<FAttackComboData_Projectile> ComboAttackDatas;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		FGameplayTag NextAttackTag;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TSubclassOf<UGameplayEffect> DamageCalcEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TSubclassOf<UGameplayEffect> StiffEffect;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Trigger" )
		bool OnlyTriggerOnce;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|StopSetting" )
		bool StopWhenAbilityEnds;

	UPROPERTY( EditDefaultsOnly, Category = "AnimMontage|Rate" )
		float AnimRootMotionTranslationScale;

	bool bNextAttack;

	int CurComboIndex;

	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Projectile" )
		TSubclassOf<AARProjectile> ProjectileClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectile" )
		FGameplayTag ProjectileFireTag;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
		float Range;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
		float Speed;
};
