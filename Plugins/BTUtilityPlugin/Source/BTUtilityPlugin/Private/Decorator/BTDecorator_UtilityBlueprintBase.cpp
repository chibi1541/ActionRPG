// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator/BTDecorator_UtilityBlueprintBase.h"

#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_UtilityBlueprintBase)

UBTDecorator_UtilityBlueprintBase::UBTDecorator_UtilityBlueprintBase( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	NodeName = TEXT( "BPUtilFunc" );

	bCreateNodeInstance = true;
}

float UBTDecorator_UtilityBlueprintBase::CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const
{
	APawn* ControlledPawn = nullptr;
	if( AIOwner )
	{
		ControlledPawn = AIOwner->GetPawn();
	}

	return CalculateUtility( AIOwner, ControlledPawn );
}

void UBTDecorator_UtilityBlueprintBase::SetOwner( AActor* Owner )
{
	ActorOwner = Owner;
	AIOwner = Cast< AAIController >( Owner );
}

#if WITH_EDITOR

bool UBTDecorator_UtilityBlueprintBase::UsesBlueprint() const
{
	return true;
}

#endif //WITH_EDITOR

