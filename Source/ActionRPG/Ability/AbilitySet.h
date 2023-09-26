// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"

#include "AbilitySet.generated.h"

USTRUCT( BlueprintType )
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY( EditDefaultsOnly, Meta = ( Category = "InputTag" ) )
	FGameplayTag InputTag;
};


UCLASS( BlueprintType, Const )
class ACTIONRPG_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UAbilitySet( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	void GiveToAbilitySystem( UAbilitySystemComponent* AbilitySystemComponent ) const;

protected:
	UPROPERTY( EditDefaultsOnly, meta = ( Category = "AbilitySet" ) )
	TArray<FAbilitySet_GameplayAbility> KeyBindingAbilities;

	UPROPERTY( EditDefaultsOnly, meta = ( Category = "AbilitySet" ) )
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};
