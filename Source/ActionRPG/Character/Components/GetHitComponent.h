// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "GetHitComponent.generated.h"

class UARAbilitySystemComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UGetHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGetHitComponent();

	virtual UAnimMontage* GetMontagetoPlay( const FVector AttackVec ) const;

	UFUNCTION( BlueprintCallable, Category = "HitReaction" )
		virtual float HitReaction( const FVector AttackVec );

	UFUNCTION( BlueprintCallable, Category = "HitReaciton" )
		const float GetReactionMontagePlayTime( const FVector AttackVec ) const;

	UFUNCTION()
		void OnHit( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
		void OnEffectDurationChange( FActiveGameplayEffect& ActiveEffect );

	bool IsBackAttack( const FVector AttackVec ) const;

protected:
	virtual void BeginPlay() override;

protected:
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;

	UPROPERTY( EditAnywhere, Category = "HitMontages|Front" )
		TObjectPtr<UAnimMontage> HitMontage_Front;

	UPROPERTY( EditAnywhere, Category = "HitMontages|Rear" )
		TObjectPtr<UAnimMontage> HitMontage_Rear;

	UPROPERTY( EditAnywhere, Category = "HitMontages|Right" )
		TObjectPtr<UAnimMontage> HitMontage_Right;

	UPROPERTY( EditAnywhere, Category = "HitMontages|Left" )
		TObjectPtr<UAnimMontage> HitMontage_Left;

	UPROPERTY( EditAnywhere, Category = "GrantingTags" )
		FGameplayTagContainer GrantingTags;

	UPROPERTY( EditAnywhere, Category = "CancelAbility" )
		FGameplayTagContainer CancelAbilityTaskTag;

private:
	UPROPERTY()
		TObjectPtr<UARAbilitySystemComponent> ASC;

	UPROPERTY()
		TObjectPtr<UAnimInstance> AnimInstance;

	FDelegateHandle GuardingHandle;

};

