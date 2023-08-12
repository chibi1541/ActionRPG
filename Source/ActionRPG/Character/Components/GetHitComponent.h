// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "GameplayTagContainer.h"

#include "GetHitComponent.generated.h"

class UARPGAbilitySystemComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UGetHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGetHitComponent();

	virtual UAnimMontage* GetMontagetoPlay( const FVector AttackVec ) const;

	UFUNCTION( BlueprintCallable, Category = "HitReaction" )
	virtual void HitReaction( const FVector AttackVec );

	UFUNCTION()
	virtual void OnMontageEnded( UAnimMontage* Montage, bool bInterrupted );

protected:
	virtual void BeginPlay() override;

	virtual bool IsBeatenNow();

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
	TObjectPtr<UARPGAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;
};

