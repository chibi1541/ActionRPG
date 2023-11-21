// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARUtilityStateComponent.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARIntRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARUtilityStateComponent)

const FName UARUtilityStateComponent::HealthRateKey( TEXT( "HealthRate" ) );
const FName UARUtilityStateComponent::StaminaRateKey( TEXT( "StaminaRate" ) );
const FName UARUtilityStateComponent::ManaRateKey( TEXT( "ManaRate" ) );
const FName UARUtilityStateComponent::ShieldGaugeRateKey( TEXT( "ShieldGaugeRate" ) );

// Sets default values for this component's properties
UARUtilityStateComponent::UARUtilityStateComponent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UARUtilityStateComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const ABaseCharacter* Owner = Cast<ABaseCharacter>( GetOwner() );
	if( !Owner )
	{
		RLOG( Error, TEXT( "Owner is not Valid" ) );
		return;
	}

	AbilitySystemComponent = Owner->GetAbilitySystemComponent();
	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "AbilitySystemComponent is not Valid : %s" ), *Owner->GetName() );
		return;
	}

	if( AbilitySystemComponent.IsValid() )
	{
		HealthAttrib = AbilitySystemComponent->GetSet<UARVitRefAttribSet>();
		ManaAttrib = AbilitySystemComponent->GetSet<UARIntRefAttribSet>();
	}
}

void UARUtilityStateComponent::InitializeOnPossessed( UBlackboardComponent* BlackboardComponent )
{
	if( !BlackboardComponent )
	{
		RLOG( Error, TEXT( "BlackboardComponent is not Valid" ) );
		return;
	}

	Blackboard = BlackboardComponent;

	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "AbilitySystemComponent is not Valid" ) );
		return;
	}

	if( HealthAttrib.IsValid() )
	{
		if( !HealthRateHandle.IsValid() )
		{
			HealthRateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetHealthAttribute() ).AddUObject( this, &UARUtilityStateComponent::OnHealthChange );
		}

		if( !StaminaRateHandle.IsValid() )
		{
			StaminaRateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetStaminaAttribute() ).AddUObject( this, &UARUtilityStateComponent::OnStaminaChange );
		}

		if( !ShieldRateHandle.IsValid() )
		{
			ShieldRateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetShieldGaugeAttribute() ).AddUObject( this, &UARUtilityStateComponent::OnShieldGaugeChange );
		}
	}
	else
	{
		RLOG( Error, TEXT( "HealthAttrib is not Valid" ) );
	}

	if( ManaAttrib.IsValid() )
	{
		if( !ManaRateHandle.IsValid() )
		{
			ManaRateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttrib->GetManaAttribute() ).AddUObject( this, &UARUtilityStateComponent::OnManaChange );
		}
	}
	else
	{
		RLOG( Error, TEXT( "ManaAttrib is not Valid" ) );
	}

}

void UARUtilityStateComponent::UnPossessController()
{
	// Release AIController
	Blackboard.Reset();

	if( AbilitySystemComponent.IsValid() )
	{
		if( HealthAttrib.IsValid() )
		{
			if( HealthRateHandle.IsValid() )
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetHealthAttribute() ).Remove( HealthRateHandle );
			}

			if( StaminaRateHandle.IsValid() )
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetStaminaAttribute() ).Remove( StaminaRateHandle );
			}

			if( ShieldRateHandle.IsValid() )
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetShieldGaugeAttribute() ).Remove( ShieldRateHandle );
			}
		}

		if( ManaAttrib.IsValid() )
		{
			if( ManaRateHandle.IsValid() )
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttrib->GetManaAttribute() ).Remove( ManaRateHandle );
			}
		}
	}

}

void UARUtilityStateComponent::OnHealthChange( const FOnAttributeChangeData& Data )
{
	if( !Blackboard.IsValid() )
	{
		RLOG( Error, TEXT( "Blackboard is not Valid" ) );
		return;
	}

	// Set New Health Rate On Blackboard
	float HPRate = 0.f;

	if( HealthAttrib.IsValid() )
	{
		if( HealthAttrib->GetMaxHealth() >= 0.f && Data.NewValue >= 0.f )
		{
			HPRate = Data.NewValue / HealthAttrib->GetMaxHealth();
		}
	}

	Blackboard->SetValueAsFloat( HealthRateKey, HPRate );
}

void UARUtilityStateComponent::OnStaminaChange( const FOnAttributeChangeData& Data )
{
	if( !Blackboard.IsValid() )
	{
		RLOG( Error, TEXT( "Blackboard is not Valid" ) );
		return;
	}

	// Set New Stamina Rate On Blackboard
	float StaminaRate = 0.f;

	if( HealthAttrib.IsValid() )
	{
		if( HealthAttrib->GetMaxStamina() >= 0.f && Data.NewValue >= 0.f )
		{
			StaminaRate = Data.NewValue / HealthAttrib->GetMaxStamina();
		}
	}

	Blackboard->SetValueAsFloat( StaminaRateKey, StaminaRate );

}

void UARUtilityStateComponent::OnManaChange( const FOnAttributeChangeData& Data )
{
	if( !Blackboard.IsValid() )
	{
		RLOG( Error, TEXT( "Blackboard is not Valid" ) );
		return;
	}

	// Set New Mana Rate On Blackboard
	float ManaRate = 0.f;

	if( ManaAttrib.IsValid() )
	{
		if( ManaAttrib->GetMaxMana() >= 0.f && Data.NewValue >= 0.f )
		{
			ManaRate = Data.NewValue / ManaAttrib->GetMaxMana();
		}
	}

	Blackboard->SetValueAsFloat( ManaRateKey, ManaRate );
}

void UARUtilityStateComponent::OnShieldGaugeChange( const FOnAttributeChangeData& Data )
{
	if( !Blackboard.IsValid() )
	{
		RLOG( Error, TEXT( "Blackboard is not Valid" ) );
		return;
	}

	// Set New ShieldGauge Rate On Blackboard
	float ShieldGaugeRate = 0.f;

	if( HealthAttrib.IsValid() )
	{
		if( HealthAttrib->GetMaxShieldGauge() >= 0.f && Data.NewValue >= 0.f )
		{
			ShieldGaugeRate = Data.NewValue / HealthAttrib->GetMaxShieldGauge();
		}
	}

	Blackboard->SetValueAsFloat( ShieldGaugeRateKey, ShieldGaugeRate );
}