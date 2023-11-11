// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTService.h"

#include "ARBTService_InAttackRange.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARBTService_InAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UARBTService_InAttackRange( const FObjectInitializer& ObjectInitializer );

protected:
	virtual void TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;

protected:
	UPROPERTY( EditAnywhere, Category = "Range" )
		float AttackRange;
};
