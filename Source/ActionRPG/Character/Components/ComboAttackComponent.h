// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"

#include "ComboAttackComponent.generated.h"

class UAbilitySystemComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UComboAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UComboAttackComponent();

protected:
	virtual void BeginPlay() override;

	void SetNextAttack();

public:
	void StartComboAttack();
	
	bool NowAttacking();

protected:
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TArray<FGameplayAbilitySpecHandle> ComboAttackHandles;

	int CurComboIndex;

	bool bNextAttack;
};
