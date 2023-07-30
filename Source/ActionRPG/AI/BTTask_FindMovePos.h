// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTask_FindMovePos.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UBTTask_FindMovePos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindMovePos( const FObjectInitializer& ObjectInitializer );

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

protected:
	UPROPERTY( EditAnywhere, Category = "Range", meta = ( AllowPrivateAccess = "true" ) )
	float SearchRadius;
};
