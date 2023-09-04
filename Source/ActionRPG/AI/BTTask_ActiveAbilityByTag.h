// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"

#include "BTTask_ActiveAbilityByTag.generated.h"

class UGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;

struct FBTActiveAbilityTaskMemory : public FBTTaskMemory
{
	FDelegateHandle BBObserverDelegateHandle;

	FDelegateHandle AbilityEndDelegateHandle;

	FGameplayAbilitySpec* AbilitySpec;
};

UCLASS()
class ACTIONRPG_API UBTTask_ActiveAbilityByTag : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()


protected:
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	virtual EBTNodeResult::Type AbortTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	virtual void OnTaskFinished( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult ) override;

	virtual void TaskEnd( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory );

	virtual uint16 GetInstanceMemorySize() const override;

	UFUNCTION()
	void OnAbilityEnded( UGameplayAbility* Ability );

	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY( EditAnywhere, Category = "AbilityInputTag" )
	FGameplayTag InputTag;
};
