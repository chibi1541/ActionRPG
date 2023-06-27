// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "ARPGAbilitySystemComponent.generated.h"

class UTagRelationship;

// Input을 막는(조작 불가능) 상태의 GameplayTag
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

protected:
	virtual void ApplyAbilityBlockAndCancelTags( const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags ) override;

protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// 키 입력 상태 유지 시에 지속적으로 작동하는 Abilities (추후에 작업)
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;


protected:
	UPROPERTY()
	TObjectPtr<UTagRelationship> TagRelationshipTable;
};
