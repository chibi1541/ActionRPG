// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ActiveAbilityByTag.h"

#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseAIController.h"
#include "Ability/ARPGGameplayAbility.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_ActiveAbilityByTag)

UBTTask_ActiveAbilityByTag::UBTTask_ActiveAbilityByTag( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = "Active Ability";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_ActiveAbilityByTag::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	RLOG( Warning, TEXT( "Call ExecuteTask. BTNode : %s" ), *GetName() );

	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	auto ASCObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject( ABaseAIController::ASCKey );
	UARPGAbilitySystemComponent* ASC = Cast<UARPGAbilitySystemComponent>( ASCObject );
	if( ASC == nullptr )
	{
		RLOG( Error, TEXT( "UBTTask_ActiveAbilityByTag::ExecuteTask failed since UARPGAbilitySystemComponent is missing : %s" ), *OwnerComp.GetAIOwner()->GetPawn()->GetName() );
		return EBTNodeResult::Failed;
	}

	if( ASC->TryActivateAbilityByInputTag( InputTag ) )
	{
		FBTActiveAbilityTaskMemory* TaskMemory = CastInstanceNodeMemory<FBTActiveAbilityTaskMemory>( NodeMemory );
		if( TaskMemory->AbilityEndDelegateHandle.IsValid() )
		{
			RLOG( Error, TEXT( "Old AbilityEndDelegateHandle is Still valid : %s" ), *OwnerComp.GetAIOwner()->GetPawn()->GetName() );
			TaskMemory->AbilityEndDelegateHandle.Reset();
		}

		if( auto Spec = ASC->GetAbilitySpecByInputTag( InputTag ) )
		{
			if( Spec->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced )
			{
				TaskMemory->AbilityEndDelegateHandle = Spec->Ability->OnGameplayAbilityEnded.AddUObject( this, &UBTTask_ActiveAbilityByTag::OnAbilityEnded );
			}
			else if( Spec->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor )
			{
				UGameplayAbility* Instance = Spec->GetPrimaryInstance();
				if( Instance->IsActive() )
				{
					TaskMemory->AbilityEndDelegateHandle = Instance->OnGameplayAbilityEnded.AddUObject( this, &UBTTask_ActiveAbilityByTag::OnAbilityEnded );
				}
			}

			TaskMemory->AbilitySpec = Spec;
		}

		NodeResult = EBTNodeResult::InProgress;
	}
	else
	{
		NodeResult = EBTNodeResult::Failed;
	}

	return NodeResult;
}

EBTNodeResult::Type UBTTask_ActiveAbilityByTag::AbortTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	RLOG( Warning, TEXT( "Call AbortTask : %s" ), *GetName() );

	TaskEnd( OwnerComp, NodeMemory );

	return Super::AbortTask( OwnerComp, NodeMemory );
}

void UBTTask_ActiveAbilityByTag::OnTaskFinished( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult )
{
	RLOG( Warning, TEXT( "Call OnTaskFinished : %s" ), *GetName() );

	TaskEnd( OwnerComp, NodeMemory );

	Super::OnTaskFinished( OwnerComp, NodeMemory, TaskResult );
}

void UBTTask_ActiveAbilityByTag::TaskEnd( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	FBTActiveAbilityTaskMemory* TaskMemory = CastInstanceNodeMemory<FBTActiveAbilityTaskMemory>( NodeMemory );
	FGameplayAbilitySpec* Spec = TaskMemory->AbilitySpec;
	if( Spec && Spec->IsActive() )
	{
		if( TaskMemory->AbilityEndDelegateHandle.IsValid() )
		{
			if( Spec->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced )
			{
				Spec->Ability->OnGameplayAbilityEnded.Remove( TaskMemory->AbilityEndDelegateHandle );
			}
			else if( Spec->Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor )
			{
				UGameplayAbility* Instance = Spec->GetPrimaryInstance();
				if( Instance->IsActive() )
				{
					Instance->OnGameplayAbilityEnded.Remove( TaskMemory->AbilityEndDelegateHandle );
				}
			}
		}
	}

	TaskMemory->AbilitySpec = nullptr;
	TaskMemory->AbilityEndDelegateHandle.Reset();
}

uint16 UBTTask_ActiveAbilityByTag::GetInstanceMemorySize() const
{
	return sizeof( FBTActiveAbilityTaskMemory );
}

void UBTTask_ActiveAbilityByTag::OnAbilityEnded( UGameplayAbility* Ability )
{
	APawn* Owner = Cast<APawn>( Ability->GetActorInfo().AvatarActor );
	if( Owner )
	{
		AAIController* AIOwner = Cast<AAIController>( Owner->GetController() );
		UBehaviorTreeComponent* OwnerComp = AIOwner ? Cast<UBehaviorTreeComponent>( AIOwner->BrainComponent ) : nullptr;
		if( OwnerComp )
		{
			RLOG( Warning, TEXT( "End BTTask : %s" ), *GetName() );
			FinishLatentTask( *OwnerComp, EBTNodeResult::Succeeded );
		}
	}
}

FString UBTTask_ActiveAbilityByTag::GetStaticDescription() const
{
	FString KeyDesc( "None" );
	if( InputTag.IsValid() )
	{
		KeyDesc = InputTag.ToString();
	}

	return FString::Printf( TEXT( "Input Tag: %s" ), *KeyDesc );
}
