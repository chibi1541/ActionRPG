// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Ability/ARPGGameplayAbility.h"

#include "GA_Avoid.generated.h"

class UAnimMontage;
/**
 *
 */
UCLASS()
class ACTIONRPG_API UGA_Avoid : public UARPGGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Avoid( const FObjectInitializer& ObjectInitializer );

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData ) override;


protected:
	// 현재 플레이어의 카메라가 바라보고 있는 방향
	UPROPERTY( BlueprintReadOnly, Transient, meta = ( AllowPrivateAccess = "true" ) )
	FVector ForwardDir;

	// 직전에 입력 받은 이동 방향
	UPROPERTY( BlueprintReadOnly, Transient, meta = ( AllowPrivateAccess = "true" ) )
	FVector InputDir;

	UPROPERTY( EditAnywhere, Category = "AnimMontage", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UAnimMontage> MontageToPlay;

	UFUNCTION()
	void OnCancelled();

	UFUNCTION()
	void OnCompleted();

	UPROPERTY( EditAnywhere, Category = "ConstantForce", meta = ( AllowPrivateAccess = "true" ) )
	float Strength;

	UPROPERTY( EditAnywhere, Category = "ConstantForce", meta = ( AllowPrivateAccess = "true" ) )
	float Duration;
};
