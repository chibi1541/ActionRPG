// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseMonster.h"

#include "Character/BaseAIController.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARAgiRefAttribSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseMonster)

ABaseMonster::ABaseMonster( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	AIControllerClass = ABaseAIController::StaticClass();

	AttackAttribSet = CreateDefaultSubobject<UARAttackAttribSet>( "ARAttackAttribSet" );
	VitRefAttribSet = CreateDefaultSubobject<UARVitRefAttribSet>( "ARVirRefAttribSet" );
	AgiRefAttribSet = CreateDefaultSubobject<UARAgiRefAttribSet>( "UARAgiRefAttribSet" );
}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	SetHealth( GetMaxHealth() );
	SetStamina( GetMaxStamina() );
	SetShieldGauge( GetMaxShieldGauge() );

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if( AgiRefAttribSet )
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate( AgiRefAttribSet->GetModifiedMoveSpeedAttribute() ).AddUObject( this, &ABaseMonster::OnSpeedChange );
	}
}

void ABaseMonster::SetHealth( float Health )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetHealth( Health );
	}
}

void ABaseMonster::SetStamina( float Stamina )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetStamina( Stamina );
	}
}

void ABaseMonster::SetShieldGauge( float ShieldGauge )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetShieldGauge( ShieldGauge );
	}
}

float ABaseMonster::GetMaxHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxHealth();
	}

	return 0.f;
}

float ABaseMonster::GetMaxStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxStamina();
	}

	return 0.f;
}

float ABaseMonster::GetMaxShieldGauge() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxShieldGauge();
	}

	return 0.f;
}

float ABaseMonster::GetMoveSpeed() const
{
	if( AgiRefAttribSet )
	{
		return AgiRefAttribSet->GetModifiedMoveSpeed();
	}

	return 0.f;
}
