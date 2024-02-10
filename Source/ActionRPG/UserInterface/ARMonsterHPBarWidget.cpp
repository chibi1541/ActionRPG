// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARMonsterHPBarWidget.h"
#include "Character/Components/ARCharacterStateComponent.h"
#include "Character/BaseMonster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMonsterHPBarWidget)


void UARMonsterHPBarWidget::InitializeWidget( UARCharacterStateComponent* StateComponent )
{
	if( StateComponent )
	{
		StateComponent->OnCurHealthChanged.AddDynamic( this, &UARMonsterHPBarWidget::OnChangedCurrentHealthPoint );
		StateComponent->OnMaxHealthChanged.AddDynamic( this, &UARMonsterHPBarWidget::OnChangedMaxHealthPoint );

		const ABaseMonster* Monster = Cast<ABaseMonster>( StateComponent->GetOwner() );

		CurHealthPoint = ( Monster ) ? Monster->GetCurrentHealth() : 0.0f;
		MaxHealthPoint = ( Monster ) ? Monster->GetMaxHealth() : 0.0f;
		SetCurrentHP( CurHealthPoint );
		SetHPRate( HealthPointRate );
		HealthPointRate = ( MaxHealthPoint != 0.f ) ? CurHealthPoint / MaxHealthPoint : 0.f;
		SetHPRate( HealthPointRate );
	}
}

void UARMonsterHPBarWidget::OnChangedCurrentHealthPoint( float OldValue, float NewValue )
{
	CurHealthPoint = NewValue;
	HealthPointRate = ( MaxHealthPoint != 0.f ) ? CurHealthPoint / MaxHealthPoint : 0.f;
	SetCurrentHP( CurHealthPoint );
	SetHPRate( HealthPointRate );
}

void UARMonsterHPBarWidget::OnChangedMaxHealthPoint( float OldValue, float NewValue )
{
	MaxHealthPoint = NewValue;
	HealthPointRate = ( MaxHealthPoint != 0.f ) ? CurHealthPoint / MaxHealthPoint : 0.f;
	SetMaxHP( MaxHealthPoint );
	SetHPRate( HealthPointRate );
}
