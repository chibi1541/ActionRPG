// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "ARPGAbilitySystemComponent.generated.h"

class UTagRelationship;

// Input�� ����(���� �Ұ���) ������ GameplayTag
ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN( TAG_AbilityInputBlocked );

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UARPGAbilitySystemComponent( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	void ClearAbilityInput();

	void AbilityInputTagPressed( const FGameplayTag& InputTag );
	void AbilityInputTagReleased( const FGameplayTag& InputTag );

	void ProcessAbilityInput( float DeltaTime, bool bGamePaused );

	void SetTagRelationshipTable( UTagRelationship* NewTable );

	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer& OutActivationRequired, OUT FGameplayTagContainer& OutActivationBlocked ) const;

protected:
	virtual void ApplyAbilityBlockAndCancelTags( const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags ) override;


protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;


protected:
	UPROPERTY()
	TObjectPtr<UTagRelationship> TagRelationshipTable;
};
