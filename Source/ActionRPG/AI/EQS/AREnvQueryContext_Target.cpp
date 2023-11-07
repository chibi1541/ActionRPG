// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/AREnvQueryContext_Target.h"

#include "Character/BaseCharacter.h"
#include "Character/Components/ARTargetingComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Character/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AREnvQueryContext_Target)

UAREnvQueryContext_Target::UAREnvQueryContext_Target( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
}

void UAREnvQueryContext_Target::ProvideContext( FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData ) const
{
	const ABaseCharacter* QueryOwner = Cast<ABaseCharacter>( QueryInstance.Owner.Get() );

	// TargetingComponent의 타겟 정보를 가져와야하는데 그러려면 Component를 또 수정해야함
	// 일단 Blackboard의 Target을 기준으로 처리

	ABaseAIController* AIController = Cast<ABaseAIController>( QueryOwner->GetController() );
	if( !AIController )
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	auto Target = ( BlackboardComponent ) ? BlackboardComponent->GetValueAsObject( ABaseAIController::TargetKey ) : nullptr;
	AActor* TargetActor = Cast<AActor>(Target);
	if( TargetActor )
	{
		UEnvQueryItemType_Actor::SetContextHelper( ContextData, TargetActor );
	}


	// 추후에 코드 재활용
	//const UARTargetingComponent* TargetingComp = ( QueryOwner ) ? QueryOwner->GetTargetingComponent() : nullptr;
	//const ABaseCharacter* Target = ( TargetingComp ) ? TargetingComp->GetTargetCharacter() : nullptr;

	//UEnvQueryItemType_Actor::SetContextHelper( ContextData, Target );
}
