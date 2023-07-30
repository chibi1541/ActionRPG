// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAIController)

const FName ABaseAIController::HomePosKey( TEXT( "HomePos" ) );
const FName ABaseAIController::PatrolPosKey( TEXT( "PatrolPos" ) );
const FName ABaseAIController::TargetKey( TEXT( "Target" ) );

ABaseAIController::ABaseAIController( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
	TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/AI/BT_Monster.BT_Monster'"));
	if(BTObject.Succeeded())
	{
		BehaviorTree = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT( "/Script/AIModule.BlackboardData'/Game/Blueprints/AI/BB_Monster.BB_Monster'" ) );
	if( BBObject.Succeeded() )
	{
		BlackboardData = BBObject.Object;
	}
}

void ABaseAIController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );

	RLOG(Warning, TEXT("BaseAIController OnPossessed"));

	UBlackboardComponent* BlackboardComponent = Blackboard;

	if( UseBlackboard( BlackboardData, BlackboardComponent ) )
	{
		Blackboard->SetValueAsVector( HomePosKey, InPawn->GetActorLocation() );

		if( RunBehaviorTree( BehaviorTree ) == false )
		{
			RLOG( Error, TEXT( "AIController couldn't run BehaviorTree!" ) );
		}
	}
}

void ABaseAIController::OnUnPossess()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>( BrainComponent );
	if( BehaviorTreeComponent != nullptr )
	{
		BehaviorTreeComponent->StopTree( EBTStopMode::Safe );
	}
}
