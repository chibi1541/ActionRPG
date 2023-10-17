// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "ARComboAttackComponent.generated.h"

class UAbilitySystemComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UARComboAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARComboAttackComponent();

protected:
	virtual void BeginPlay() override;

	void SetNextAttack();

	void StartNextAttack( const FGameplayEventData* Payload );

	void ActivateCurrentIndexAttack();

	UFUNCTION( BlueprintCallable )
	void RemoveDelegate();

public:
	void StartComboAttack();

	const bool NowAttacking() const;

	void AddAttackAbilityHandle( const FGameplayAbilitySpecHandle& AttackHandle );

protected:
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TArray<FGameplayAbilitySpecHandle> ComboAttackHandles;

	int CurComboIndex;

	bool bNextAttack;

	FDelegateHandle DelegateHandle;
};
