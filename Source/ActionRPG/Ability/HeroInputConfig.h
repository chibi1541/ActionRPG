// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "HeroInputConfig.generated.h"

class UInputAction;

USTRUCT( BlueprintType )
struct FHeroInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		bool bTriggerWhenPaused;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Meta = ( Categories = "InputTag" ) )
	FGameplayTag InputTag;

};

/**
 *
 */
UCLASS( BlueprintType, Const )
class ACTIONRPG_API UHeroInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UHeroInputConfig( const FObjectInitializer& Objectinitializer );

	UFUNCTION( BlueprintCallable, Category = "Hero|Input" )
	const UInputAction* FindNativeInputActionForTag( const FGameplayTag& InputTag, bool bLogNotFound ) const;

	UFUNCTION( BlueprintCallable, Category = "Hero|Input" )
	const UInputAction* FindAbilityInputActionForTag( const FGameplayTag& InputTag, bool bLogNotFound ) const;

	UFUNCTION( BlueprintCallable, Category = "Hero|Input" )
	bool IsTriggerWhenPaused( const FGameplayTag& InputTag, bool bLogNotFound ) const;

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Meta = ( TitleProperty = "InputAction" ) )
	TArray<FHeroInputAction> NativeInputActions;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Meta = ( TitleProperty = "InputAction" ) )
	TArray<FHeroInputAction> AbilityInputActions;

};
