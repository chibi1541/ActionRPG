// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARUtilityStateComponent.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARIntRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARUtilityStateComponent)

// Sets default values for this component's properties
UARUtilityStateComponent::UARUtilityStateComponent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UARUtilityStateComponent::BeginPlay()
{
	Super::BeginPlay();

	const ABaseCharacter* Owner = Cast<ABaseCharacter>( GetOwner() );
	if( !Owner )
	{
		RLOG( Error, TEXT( "Owner is not Valid" ) );
		return;
	}

	const auto AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	if( !AbilitySystemComponent )
	{
		RLOG( Error, TEXT( "AbilitySystemComponent is not Valid" ) );
		return;
	}

	HealthAttrib = AbilitySystemComponent->GetSet<UARVitRefAttribSet>();
	ManaAttrib = AbilitySystemComponent->GetSet<UARIntRefAttribSet>();

}

void UARUtilityStateComponent::InitializeOnPossessed( UBlackboardComponent* BlackboardComponent )
{
	if( !BlackboardComponent )
	{
		RLOG( Error, TEXT( "BlackboardComponent is not Valid" ) );
		return;
	}

	Blackboard = BlackboardComponent;
}

void UARUtilityStateComponent::UnPossessController()
{
	// Release AIController
	Blackboard.Reset();
}

void UARUtilityStateComponent::OnHealthChange( const FOnAttributeChangeData& Data )
{
	// Set New Health Rate On Blackboard
	float HpRate = 0.f;

	if( HealthAttrib.IsValid() )
	{
		if( HealthAttrib->GetMaxHealth() >= 0.f && Data.NewValue >= 0.f )
		{
			HpRate = Data.NewValue / HealthAttrib->GetMaxHealth();
		}


	}
}

void UARUtilityStateComponent::OnStaminaChange( const FOnAttributeChangeData& Data )
{

}

void UARUtilityStateComponent::OnManaChange( const FOnAttributeChangeData& Data )
{

}

void UARUtilityStateComponent::OnShieldGaugeChange( const FOnAttributeChangeData& Data )
{

}