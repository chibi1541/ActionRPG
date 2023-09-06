// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "ARGASEnumDef.h"

#include "ARAbilitySystemComponent.generated.h"

class UTagRelationship;

ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN( TAG_AbilityInputBlocked );

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnGameplayTagCallbackDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGameplayEffectCallbackDelegate, const FGameplayEffectSpec&, EffectSpec );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnEffectDurationChangeCallbackDelegate, struct FActiveGameplayEffect&, ActiveEffect );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGameplayEffectRemovedCallbackDelegate, const FActiveGameplayEffect&, ActiveEffect );

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UARAbilitySystemComponent( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	void ClearAbilityInput();

	void AbilityInputTagPressed( const FGameplayTag& InputTag );
	void AbilityInputTagReleased( const FGameplayTag& InputTag );

	void ProcessAbilityInput( float DeltaTime, bool bGamePaused );

	void SetTagRelationshipTable( UTagRelationship* NewTable );

	void GetAdditionalActivationTagRequirements( const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer& OutActivationRequired, OUT FGameplayTagContainer& OutActivationBlocked ) const;

	// For AI
	bool TryActivateAbilityByInputTag( const FGameplayTag& InputTag );

	FGameplayAbilitySpec* GetAbilitySpecByInputTag( const FGameplayTag& InputTag );


protected:
	virtual void ApplyAbilityBlockAndCancelTags( const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags ) override;

	UFUNCTION( BlueprintCallable, Category = "Abilities" )
	virtual void CancelAbilitiseWithTags( const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags );

	virtual void OnTagUpdated( const FGameplayTag& Tag, bool TagExists ) override;

	virtual void OnActiveGameEffect( UAbilitySystemComponent* Owner, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle );

	virtual void OnGameplayEffectDurationChange( struct FActiveGameplayEffect& ActiveEffect ) override;
	
	UFUNCTION()
	void OnGameplayEffectRemoved( const FActiveGameplayEffect& ActiveEffect );

public:

	TMap<FGameplayTag, FOnGameplayTagCallbackDelegate> GameplayTagsCallBacks;

	TMap<EGameplayEffectDelegateType, FOnGameplayEffectCallbackDelegate> ActiveGameplayEffectCallBacks;

	TMap<EGameplayEffectDelegateType, FOnEffectDurationChangeCallbackDelegate> GameplayEffectDurationChangeCallBacks;

	TMap<EGameplayEffectDelegateType, FOnGameplayEffectRemovedCallbackDelegate> GameplayEffectRemoveCallBacks;

protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

protected:
	UPROPERTY()
	TObjectPtr<UTagRelationship> TagRelationshipTable;
};
