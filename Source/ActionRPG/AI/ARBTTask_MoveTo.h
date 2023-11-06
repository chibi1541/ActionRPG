// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ARBTTask_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARBTTask_MoveTo : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
