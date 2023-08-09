// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AI/BTTask_ActiveAbilityByTag.h"

#include "BTTask_ActiveAbilityPressRepeat.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UBTTask_ActiveAbilityPressRepeat : public UBTTask_ActiveAbilityByTag
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	virtual void TickTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;


private:

	UPROPERTY( EditAnywhere, Category = "TickNotify" )
	bool UseTickEvent;

	UPROPERTY( EditAnywhere, Category = "TickInterval" )
	float TickInterval;

};
