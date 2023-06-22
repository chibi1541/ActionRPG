// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "ARPGAbilitySystemComponent.generated.h"

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
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

protected:
	
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Ű �Է� ���� ���� �ÿ� ���������� �۵��ϴ� Abilities (���Ŀ� �۾�)
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

};
