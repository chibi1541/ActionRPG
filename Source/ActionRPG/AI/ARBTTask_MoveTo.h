// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "ARBTTask_MoveTo.generated.h"

class AActor;
class UNavigationSystemV1;

UCLASS()
class ACTIONRPG_API UARBTTask_MoveTo : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()


private:
	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

protected:
	virtual void TickTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;

	virtual FVector CalcMovement( const AActor* Owner );

protected:
	FVector TargetLocation;

	TObjectPtr<UNavigationSystemV1> NaviSystme;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
		float Radius = 50.0f;
};
