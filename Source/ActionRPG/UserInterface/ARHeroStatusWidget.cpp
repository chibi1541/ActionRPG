// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARHeroStatusWidget.h"

#include "Character/Components/ARCharacterStateComponent.h"
#include "Character/HeroCharacter.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ProgressBar.h"

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

	LevelText->SetText( FText::Format( LOCTEXT( "Status_Level", "LV {0}" ), ( Hero ) ? Hero->GetCharacterLevel() : 0 ) );

	EmblemSwitcher->SetActiveWidgetIndex( ( Hero ) ? static_cast< int >( Hero->GetHeroClass() ) : 0 );

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
	UpdateManaText();
	UpdateStaminaText();
	UpdateShieldGaugeText();
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
	UpdateManaText();
}

void UARHeroStatusWidget::OnChangedMaxManaPoint( float OldValue, float NewValue )
{
	MaxManaPoint = NewValue;
	ChangeManaPointRate();
	UpdateManaText();
}

void UARHeroStatusWidget::OnChangedCurrentStaminaPoint( float OldValue, float NewValue )
{
	CurStaminaPoint = NewValue;
	ChangeStaminaPointRate();
	UpdateStaminaText();
}

void UARHeroStatusWidget::OnChangedMaxStaminaPoint( float OldValue, float NewValue )
{
	MaxStaminaPoint = NewValue;
	ChangeStaminaPointRate();
	UpdateStaminaText();
}

void UARHeroStatusWidget::OnChangedCurrentShieldGaugePoint( float OldValue, float NewValue )
{
	CurShieldGaugePoint = NewValue;
	ChangeShieldGaugePointRate();
	UpdateShieldGaugeText();
}

void UARHeroStatusWidget::OnChangedMaxShieldGaugePoint( float OldValue, float NewValue )
{
	MaxShieldGaugePoint = NewValue;
	ChangeShieldGaugePointRate();
	UpdateShieldGaugeText();
}

FORCEINLINE void UARHeroStatusWidget::ChangeHealthPointRate()
{
	HealthPointRate = ( MaxHealthPoint != 0.f ) ? CurHealthPoint / MaxHealthPoint : 0.f;
	HPBar->SetPercent( HealthPointRate );
}

FORCEINLINE void UARHeroStatusWidget::ChangeManaPointRate()
{
	ManaPointRate = ( MaxManaPoint != 0.f ) ? CurManaPoint / MaxManaPoint : 0.f;
	ManaBar->SetPercent( ManaPointRate );
}

FORCEINLINE void UARHeroStatusWidget::ChangeStaminaPointRate()
{
	StaminaPointRate = ( MaxStaminaPoint != 0.f ) ? CurStaminaPoint / MaxStaminaPoint : 0.f;
	StaminaBar->SetPercent( StaminaPointRate );

}

FORCEINLINE void UARHeroStatusWidget::ChangeShieldGaugePointRate()
{
	ShieldGaugePointRate = ( MaxShieldGaugePoint != 0.f ) ? CurShieldGaugePoint / MaxShieldGaugePoint : 0.f;
	ShieldGaugeBar->SetPercent( ShieldGaugePointRate );
}

void UARHeroStatusWidget::UpdateHPText()
{
	// HP 올림 처리
	int CurHPValue = FMath::CeilToInt( CurHealthPoint );
	int MaxHPValue = FMath::CeilToInt( MaxHealthPoint );

	HPText->SetText( FText::Format( LOCTEXT( "Current/Max_Status", "{0} / {1}" ), CurHPValue, MaxHPValue ) );
}

void UARHeroStatusWidget::UpdateManaText()
{
	int CurManaValue = FMath::CeilToInt( CurManaPoint );
	int MaxManaValue = FMath::CeilToInt( MaxManaPoint );

	ManaText->SetText( FText::Format( LOCTEXT( "Current/Max_Status", "{0} / {1}" ), CurManaValue, MaxManaValue ) );
}

void UARHeroStatusWidget::UpdateShieldGaugeText()
{
	int CurShieldGaugeValue = FMath::CeilToInt( CurShieldGaugePoint );
	int MaxShieldGaugeValue = FMath::CeilToInt( MaxShieldGaugePoint );

	ShieldGaugeText->SetText( FText::Format( LOCTEXT( "Current/Max_Status", "{0} / {1}" ), CurShieldGaugeValue, MaxShieldGaugeValue ) );
}

void UARHeroStatusWidget::UpdateStaminaText()
{
	int CurStaminaValue = FMath::CeilToInt( CurStaminaPoint );
	int MaxStaminaValue = FMath::CeilToInt( MaxStaminaPoint );

	StaminaText->SetText( FText::Format( LOCTEXT( "Current/Max_Status", "{0} / {1}" ), CurStaminaValue, MaxStaminaValue ) );
}

#undef LOCTEXT_NAMESPACE