// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTTask_MoveTo.h"

#include "GameFramework/Actor.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ARBTTask_MoveTo.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARBTTask_MoveTo)

UARBTTask_MoveTo::UARBTTask_MoveTo( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "CustomMoveTo" );
	bNotifyTick = true;

	BlackboardKey.AddObjectFilter( this, GET_MEMBER_NAME_CHECKED( UARBTTask_MoveTo, BlackboardKey ), APawn::StaticClass() );
	BlackboardKey.AddVectorFilter( this, GET_MEMBER_NAME_CHECKED( UARBTTask_MoveTo, BlackboardKey ) );

}

EBTNodeResult::Type UARBTTask_MoveTo::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	TargetLocation = FVector::ZeroVector;
	auto Blackboard = OwnerComp.GetBlackboardComponent();;
	if( BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() )
	{
		auto KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>( BlackboardKey.GetSelectedKeyID() );
		auto TargetActor = Cast<AActor>( KeyValue );
		if( TargetActor )
		{
			TargetLocation = TargetActor->GetActorLocation();
		}
	}
	else if( BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() )
	{
		TargetLocation = Blackboard->GetValue<UBlackboardKeyType_Vector>( BlackboardKey.GetSelectedKeyID() );
	}

	if( TargetLocation == FVector::ZeroVector )
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UARBTTask_MoveTo::TickTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	auto Controller = OwnerComp.GetAIOwner();
	auto Owner = Controller->GetPawn();

	auto Distance = Owner->GetActorLocation() - TargetLocation;
	if( Distance.SquaredLength() > Radius * Radius )
	{
		auto Movement = CalcMovement( Owner );

		if( !Movement.IsZero() )
		{
			Owner->AddMovementInput( Movement );
		}
	}
	else
	{
		FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
	}

	return;
}

FVector UARBTTask_MoveTo::CalcMovement( const AActor* Owner )
{
	auto Path = NaviSystme->FindPathToLocationSynchronously( GetWorld(), Owner->GetActorLocation(), TargetLocation );
	if( !Path )
	{
		return FVector::Zero();
	}

	auto& FindPathPoints = Path->PathPoints;

	if( FindPathPoints.Num() >= 2 )
	{
		auto Dir = FindPathPoints[1] - FindPathPoints[0];
		Dir.Normalize();
		return Dir;
	}
	else
	{
		return FVector::Zero();
	}
}
