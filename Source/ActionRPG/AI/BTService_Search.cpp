// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Search.h"

#include "Character/BaseAIController.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "Character/Components/ARCharacterStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTService_Search)

UBTService_Search::UBTService_Search( const FObjectInitializer& ObjectInitializer )
{
	NodeName = TEXT( "Search" );
	Interval = 1.0f;
	SearchRadius = 600.f;
}

void UBTService_Search::TickNode( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds )
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds );

	const APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if( !ControllingPawn )
	{
		return;
	}

	const ABaseCharacter* Owener = Cast<ABaseCharacter>( ControllingPawn );
	if( !Owener )
	{
		return;
	}

	auto TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject( ABaseAIController::TargetKey );
	auto Target = Cast<ABaseCharacter>( TargetObject );

	// Cast 후에 Dead 상태인지를 판단하는 하도록 추후에 변경
	if( Target )
	{
		double Distance = FVector::Dist( ControllingPawn->GetActorLocation(), Target->GetActorLocation() );
		if( Distance <= SearchRadius )
		{
			return;
		}
	}

	const UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	if( World == nullptr )
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam( NAME_None, false, ControllingPawn );
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere( SearchRadius ),
		CollisionQueryParam
	);

	if( bResult )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject( ABaseAIController::TargetKey, nullptr );
		for( const FOverlapResult OverlapResult : OverlapResults )
		{
			ABaseCharacter* CheckCharacter = Cast<ABaseCharacter>( OverlapResult.GetActor() );
			if( CheckCharacter )
			{
				if( Owener->GetCharacterType() == CheckCharacter->GetCharacterType() )
				{
					continue;
				}

				auto StateComponent = CheckCharacter->GetCharacterStateComponenet();
				if( StateComponent && StateComponent->GetDeadState() )
				{
					continue;
				}

				OwnerComp.GetBlackboardComponent()->SetValueAsObject( ABaseAIController::TargetKey, CheckCharacter );
				DrawDebugSphere( World, Center, SearchRadius, 16, FColor::Green, false, 0.3f );
				DrawDebugPoint( World, CheckCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.3f );
				DrawDebugLine( World, ControllingPawn->GetActorLocation(), CheckCharacter->GetActorLocation(), FColor::Blue, false, 0.3f );
				return;
			}
		}
	}

	DrawDebugSphere( World, Center, SearchRadius, 16, FColor::Red, false, 0.3f );
}
