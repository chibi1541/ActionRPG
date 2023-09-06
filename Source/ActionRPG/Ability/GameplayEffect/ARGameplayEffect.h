// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ARGASEnumDef.h"

#include "ARGameplayEffect.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGameplayEffect : public UGameplayEffect
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "DelegateType" )
	EGameplayEffectDelegateType DelegateType;


public:
	FORCEINLINE EGameplayEffectDelegateType GetDelegateType() const { return DelegateType; }
};
