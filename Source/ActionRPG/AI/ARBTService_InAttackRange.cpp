// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTService_InAttackRange.h"

#include "Character/BaseAIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

#if WITH_EDITOR

#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#endif


#include UE_INLINE_GENERATED_CPP_BY_NAME(ARBTService_InAttackRange)


UARBTService_InAttackRange::UARBTService_InAttackRange( const FObjectInitializer& ObjectInitializer )
{
	NodeName = TEXT( "InAttackRange" );
	Interval = 0.5f;
	AttackRange = 300.f;
}

void UARBTService_InAttackRange::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	const auto OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject( ABaseAIController::TargetKey );
	const auto Target = Cast<APawn>( TargetObject );

	double Distance = FVector::Dist( OwnerPawn->GetActorLocation(), Target->GetActorLocation() );
	bool bInAttackRange = Distance <= AttackRange;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool( ABaseAIController::InAttackRangeKey, bInAttackRange );

#if WITH_EDITOR
	DrawDebugSphere( OwnerPawn->GetWorld(), OwnerPawn->GetActorLocation(), AttackRange, 16, FColor::Red, false, 0.3f );
#endif
}


