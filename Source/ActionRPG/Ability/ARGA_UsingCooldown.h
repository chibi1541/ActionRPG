// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_SpendStamina.h"

#include "ARGA_UsingCooldown.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGA_UsingCooldown : public UARGA_SpendStamina
{
	GENERATED_UCLASS_BODY()


protected:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Cooldown" )
		FScalableFloat CooldownDuration;

};
