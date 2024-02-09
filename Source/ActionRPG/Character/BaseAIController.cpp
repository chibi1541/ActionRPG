// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/Components/ARUtilityStateComponent.h"
#include "Character/HeroCharacter.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseAIController)

const FName ABaseAIController::HomePosKey( TEXT( "HomePos" ) );
const FName ABaseAIController::PatrolPosKey( TEXT( "PatrolPos" ) );
const FName ABaseAIController::TargetKey( TEXT( "Target" ) );
const FName ABaseAIController::ASCKey( TEXT( "ASC" ) );
const FName ABaseAIController::InAttackRangeKey( TEXT( "InAttackRange" ) );

ABaseAIController::ABaseAIController( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if( UWorld* World = GetWorld() )
	{
		for( TActorIterator<AHeroCharacter> ActorIter( World ); ActorIter; ++ActorIter )
		{
			AHeroCharacter* Hero = *ActorIter;
			Blackboard->SetValueAsObject( TargetKey, Hero );
		}
	}
}

void ABaseAIController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );

	RLOG( Warning, TEXT( "BaseAIController OnPossessed" ) );

	UBlackboardComponent* BlackboardComponent = Blackboard;

	if( UseBlackboard( BlackboardData, BlackboardComponent ) )
	{
		Blackboard->SetValueAsVector( HomePosKey, InPawn->GetActorLocation() );

		const auto AICharacter = CastChecked<ABaseCharacter>( InPawn );
		if( UARAbilitySystemComponent* ArASC = AICharacter->GetARAbilitySystemComponent() )
		{
			Blackboard->SetValueAsObject( ASCKey, ArASC );
		}

		if( RunBehaviorTree( BehaviorTree ) == false )
		{
			RLOG( Error, TEXT( "AIController couldn't run BehaviorTree!" ) );
		}
	}

	const auto BaseCharacter = Cast<ABaseCharacter>( InPawn );
	if( BaseCharacter )
	{
		auto ActorComp = BaseCharacter->GetComponentByClass( UARUtilityStateComponent::StaticClass() );
		if( ActorComp )
		{
			auto UtilityStateComp = Cast<UARUtilityStateComponent>( ActorComp );
			if( UtilityStateComp )
			{
				UtilityStateComp->InitializeOnPossessed( Blackboard );
			}
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

	const auto BaseCharacter = ( GetCharacter() ) ? Cast<ABaseCharacter>( GetCharacter() ) : nullptr;
	if( BaseCharacter )
	{
		auto ActorComp = BaseCharacter->GetComponentByClass( UARUtilityStateComponent::StaticClass() );
		if( ActorComp )
		{
			auto UtilityStateComp = Cast<UARUtilityStateComponent>( ActorComp );
			if( UtilityStateComp )
			{
				UtilityStateComp->UnPossessController();
			}
		}
	}
}

UARAbilitySystemComponent* ABaseAIController::GetARAbilitySystemComponent() const
{
	const ABaseCharacter* AICharacter = CastChecked<ABaseCharacter>( GetCharacter() );
	return ( AICharacter ? AICharacter->GetARAbilitySystemComponent() : nullptr );
}

