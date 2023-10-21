// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"
#include "Character/ARCharacterEnumDef.h"

#include "GameplayEffectSet.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 *
 */
UCLASS( BlueprintType, Const )
class ACTIONRPG_API UGameplayEffectSet : public UDataAsset
{
	GENERATED_BODY()

public:
	void AttribInitialize( UAbilitySystemComponent* AbilitySystemComponent ) const;

	void PassiveInitialize( UAbilitySystemComponent* AbilitySystemComponent ) const;

protected:
	UPROPERTY( EditDefaultsOnly, meta = ( Category = "GameplayEffectSet" ) )
		TSubclassOf<UGameplayEffect> BaseInitializer;

	UPROPERTY( EditDefaultsOnly, meta = ( Category = "GameplayEffectSet" ) )
		TArray< TSubclassOf<UGameplayEffect>> AttribInitializers;

	UPROPERTY( EditDefaultsOnly, meta = ( Category = "GameplayEffectSet" ) )
		TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;
};
