// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ActiveAbilityPressRepeat.h"

#include "Character/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Ability/ARPGGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_ActiveAbilityPressRepeat)

UBTTask_ActiveAbilityPressRepeat::UBTTask_ActiveAbilityPressRepeat( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = "Active Ability Press Repeat";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_ActiveAbilityPressRepeat::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	if( NodeResult == EBTNodeResult::InProgress )
	{
		bNotifyTick = UseTickEvent;
		bTickIntervals = TickInterval > 0.f;
		SetNextTickTime( NodeMemory, TickInterval );
	}

	return NodeResult;
}

void UBTTask_ActiveAbilityPressRepeat::TickTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	auto ASCObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject( ABaseAIController::ASCKey );
	UARPGAbilitySystemComponent* ASC = Cast<UARPGAbilitySystemComponent>( ASCObject );
	if( ASC == nullptr )
	{
		RLOG( Error, TEXT( "UBTTask_ActiveAbilityByTag::TickTask failed since UARPGAbilitySystemComponent is missing : %s" ), *OwnerComp.GetAIOwner()->GetPawn()->GetName() );
		return;
	}

	FBTActiveAbilityTaskMemory* TaskMemory = CastInstanceNodeMemory<FBTActiveAbilityTaskMemory>( NodeMemory );
	FGameplayAbilitySpec* Spec = TaskMemory->AbilitySpec;

	if( !Spec || !Spec->IsActive() )
	{
		RLOG( Error, TEXT( "UBTTask_ActiveAbilityByTag::TickTask AbilitySpec is missing or Not Active : %s" ), *OwnerComp.GetAIOwner()->GetPawn()->GetName() );
		AbortTask( OwnerComp, NodeMemory );
		return;
	}

	ASC->AbilitySpecInputPressed( *Spec );

	SetNextTickTime( NodeMemory, TickInterval );
}