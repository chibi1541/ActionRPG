// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARHeroStatusWidget.h"

#include "Character/Components/ARCharacterStateComponent.h"
#include "Character/HeroCharacter.h"
#include "UserInterface/ARResizeTextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARHeroStatusWidget)

#define LOCTEXT_NAMESPACE "MainGameHUD"

UARHeroStatusWidget::UARHeroStatusWidget( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{

}

void UARHeroStatusWidget::InitializeStatusWidget( UARCharacterStateComponent* StateComponent )
{
	if( !StateComponent )
	{
		// show error log
		return;
	}

	HeroStateComponent = StateComponent;
	HeroStateComponent->OnCurHealthChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedCurrentHealthPoint );
	HeroStateComponent->OnMaxHealthChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedMaxHealthPoint );

	HeroStateComponent->OnCurManaChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedCurrentManaPoint );
	HeroStateComponent->OnMaxManaChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedMaxManaPoint );

	HeroStateComponent->OnCurStaminaChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedCurrentStaminaPoint );
	HeroStateComponent->OnMaxStaminaChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedMaxStaminaPoint );

	HeroStateComponent->OnCurShieldGaugeChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedCurrentShieldGaugePoint );
	HeroStateComponent->OnMaxShieldGaugeChanged.AddDynamic( this, &UARHeroStatusWidget::OnChangedMaxShieldGaugePoint );


	const AHeroCharacter* Hero = Cast<AHeroCharacter>( HeroStateComponent->GetOwner() );

	CurHealthPoint = ( Hero ) ? Hero->GetCurrentHealth() : 0.0f;
	MaxHealthPoint = ( Hero ) ? Hero->GetMaxHealth() : 0.0f;

	ChangeHealthPointRate();

	CurManaPoint = ( Hero ) ? Hero->GetCurrentMana() : 0.0f;
	MaxManaPoint = ( Hero ) ? Hero->GetMaxMana() : 0.0f;

	ChangeManaPointRate();

	CurStaminaPoint = ( Hero ) ? Hero->GetCurrentStamina() : 0.0f;
	MaxStaminaPoint = ( Hero ) ? Hero->GetMaxStamina() : 0.0f;

	ChangeStaminaPointRate();

	CurShieldGaugePoint = ( Hero ) ? Hero->GetCurrentShieldGauge() : 0.0f;
	MaxShieldGaugePoint = ( Hero ) ? Hero->GetMaxShieldGauge() : 0.0f;

	ChangeShieldGaugePointRate();

	UpdateHPText();
}

void UARHeroStatusWidget::OnChangedCurrentHealthPoint( float OldValue, float NewValue )
{
	CurHealthPoint = NewValue;
	ChangeHealthPointRate();
	UpdateHPText();
}

void UARHeroStatusWidget::OnChangedMaxHealthPoint( float OldValue, float NewValue )
{
	MaxHealthPoint = NewValue;
	ChangeHealthPointRate();
	UpdateHPText();
}

void UARHeroStatusWidget::OnChangedCurrentManaPoint( float OldValue, float NewValue )
{
	CurManaPoint = NewValue;
	ChangeManaPointRate();
}

void UARHeroStatusWidget::OnChangedMaxManaPoint( float OldValue, float NewValue )
{
	MaxManaPoint = NewValue;
	ChangeManaPointRate();
}

void UARHeroStatusWidget::OnChangedCurrentStaminaPoint( float OldValue, float NewValue )
{
	CurStaminaPoint = NewValue;
	ChangeStaminaPointRate();
}

void UARHeroStatusWidget::OnChangedMaxStaminaPoint( float OldValue, float NewValue )
{
	MaxStaminaPoint = NewValue;
	ChangeStaminaPointRate();
}

void UARHeroStatusWidget::OnChangedCurrentShieldGaugePoint( float OldValue, float NewValue )
{
	CurShieldGaugePoint = NewValue;
	ChangeShieldGaugePointRate();
}

void UARHeroStatusWidget::OnChangedMaxShieldGaugePoint( float OldValue, float NewValue )
{
	MaxShieldGaugePoint = NewValue;
	ChangeShieldGaugePointRate();
}

FORCEINLINE void UARHeroStatusWidget::ChangeHealthPointRate()
{
	HealthPointRate = ( MaxHealthPoint != 0.f ) ? CurHealthPoint / MaxHealthPoint : 0.f;
}

FORCEINLINE void UARHeroStatusWidget::ChangeManaPointRate()
{
	ManaPointRate = ( MaxManaPoint != 0.f ) ? CurManaPoint / MaxManaPoint : 0.f;
}

FORCEINLINE void UARHeroStatusWidget::ChangeStaminaPointRate()
{
	StaminaPointRate = ( MaxStaminaPoint != 0.f ) ? CurStaminaPoint / MaxStaminaPoint : 0.f;

}

FORCEINLINE void UARHeroStatusWidget::ChangeShieldGaugePointRate()
{
	ShieldGaugePointRate = ( MaxShieldGaugePoint != 0.f ) ? CurShieldGaugePoint / MaxShieldGaugePoint : 0.f;

}

void UARHeroStatusWidget::UpdateHPText()
{
	// HP 올림 처리
	int CurHPValue = FMath::CeilToInt( CurHealthPoint );
	int MaxHPValue = FMath::CeilToInt( MaxHealthPoint );

	HPText->SetText( FText::Format( LOCTEXT( "Current/Max_Status", "{0}/{1}" ), CurHPValue, MaxHPValue ) );
}

#undef LOCTEXT_NAMESPACE