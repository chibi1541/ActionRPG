// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_ActiveAbilityByTag.generated.h"

class UARPGAbilitySystemComponent;
class UGameplayAbility;
struct FGameplayTag;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UBTTask_ActiveAbilityByTag : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	UFUNCTION()
	void OnAbilityEnded( UGameplayAbility* Ability );


protected:
	UPROPERTY( EditAnywhere, Category = "AbilityInputTag" )
		FGameplayTag InputTag;

	UPROPERTY( VisibleAnywhere, Category = "AbilitySystem" )
		TObjectPtr<UARPGAbilitySystemComponent> ASC;

};
