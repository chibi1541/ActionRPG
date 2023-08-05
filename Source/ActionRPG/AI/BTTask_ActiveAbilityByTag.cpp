// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ActiveAbilityByTag.h"

#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Ability/ARPGGameplayAbility.h"
#include "AIController.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_ActiveAbilityByTag)


UBTTask_ActiveAbilityByTag::UBTTask_ActiveAbilityByTag( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = "Active Ability";
}

EBTNodeResult::Type UBTTask_ActiveAbilityByTag::ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory )
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	ABaseCharacter* Character = Cast<ABaseCharacter>( OwnerComp.GetAIOwner()->GetPawn() );
	if( Character == nullptr )
	{
		RLOG( Error, TEXT( "Cast ABaseCharacter Fail : %s" ), *OwnerComp.GetAIOwner()->GetPawn()->GetName() );
		return EBTNodeResult::Failed;
	}

	ASC = Character->GetARPGAbilitySystemComponent();
	if( ASC )
	{
		if( ASC->TryActivateAbilityByInputTag( InputTag ) )
		{
			FGameplayAbilitySpec OutSpec;
			if( ASC->GetAbilitySpecByInputTag( InputTag, OutSpec ) )
			{
				if( OutSpec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced )
				{
					OutSpec.Ability->OnGameplayAbilityEnded.AddUObject( this, &UBTTask_ActiveAbilityByTag::OnAbilityEnded );
				}
				else
				{
					TArray<UGameplayAbility*> Instances = OutSpec.GetAbilityInstances();
					for( UGameplayAbility* Instance : Instances )
					{
						Instance->OnGameplayAbilityEnded.AddUObject( this, &UBTTask_ActiveAbilityByTag::OnAbilityEnded );
					}
				}
			}

			NodeResult = EBTNodeResult::InProgress;
		}
		else
		{
			NodeResult = EBTNodeResult::Failed;
		}
	}
	else
	{
		NodeResult = EBTNodeResult::Failed;
	}

	return NodeResult;
}

void UBTTask_ActiveAbilityByTag::OnAbilityEnded( UGameplayAbility* Ability )
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>( GetOuter() );

	if( OwnerComp )
	{
		FinishLatentTask( *OwnerComp, EBTNodeResult::Succeeded );
	}
}

