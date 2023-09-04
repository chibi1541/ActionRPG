// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Ability/ARGameplayAbility.h"

#include "GA_UsedTraceHit.generated.h"

class UHitTraceComponent;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UGA_UsedTraceHit : public UARGameplayAbility
{
	GENERATED_UCLASS_BODY()



protected:
	virtual void OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec ) override;

	UFUNCTION( BlueprintCallable, Category = "HitTrace|OnHit|Init" )
	virtual void InitDelegateOnHit();

	UFUNCTION( BlueprintNativeEvent, Category = "HitTrace|OnHit" )
		void OnHit( FHitResult HitResult );

	UFUNCTION( BlueprintCallable, Category = "HitTrace|OnHit" )
		virtual void ResetHitTraceComp();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "HitTrace" )
		TObjectPtr<UHitTraceComponent> HitTraceComp;



};
