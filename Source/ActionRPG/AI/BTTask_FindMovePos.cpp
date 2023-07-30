// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindMovePos.h"
#include "NavigationSystem.h"
#include "Character/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_FindMovePos)

UBTTask_FindMovePos::UBTTask_FindMovePos( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "FindMovePos" );
	SearchRadius = 300.f;
}

EBTNodeResult::Type UBTTask_FindMovePos::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	EBTNodeResult::Type Result = Super::ExecuteTask( OwnerComp, NodeMemory );

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( ControllingPawn == nullptr )
		return EBTNodeResult::Failed;

	UWorld* World = ControllingPawn->GetWorld();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem( World );
	if( NavSystem == nullptr )
		return EBTNodeResult::Failed;

	const FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector( ABaseAIController::HomePosKey );
	FNavLocation NextMove;

	if( NavSystem->GetRandomPointInNavigableRadius( Origin, SearchRadius, NextMove ) )
	{
		DrawDebugSphere( World, Origin, SearchRadius, 16, FColor::Cyan, false, 0.3f );
		DrawDebugPoint( World, NextMove.Location, 10.0f, FColor::Cyan, false, 0.3f );

		OwnerComp.GetBlackboardComponent()->SetValueAsVector( ABaseAIController::PatrolPosKey, NextMove.Location );
		return EBTNodeResult::Succeeded;
	}

	return Result;
}
