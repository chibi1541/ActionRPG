// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTService.h"

#include "BTService_Search.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UBTService_Search : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Search( const FObjectInitializer& ObjectInitializer );

protected:
	virtual void TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds ) override;

	UPROPERTY( EditAnywhere, Category = "Range", meta = ( AllowPrivateAccess = "true" ) )
	float SearchRadius;
};
