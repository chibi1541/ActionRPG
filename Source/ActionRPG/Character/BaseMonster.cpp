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

void ABaseMonster::InitializerAttributes()
{
	Super::InitializerAttributes();

	if( !HealthAttribInitializer )
	{
		RLOG( Error, TEXT( "HealthAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle HealthHandle = AbilitySystemComp->MakeOutgoingSpec( HealthAttribInitializer, GetCharacterLevel(), EffectContext );
	if( HealthHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *HealthHandle.Data.Get(), AbilitySystemComp.Get() );
	}

	if( !AttackAttribInitializer )
	{
		RLOG( Error, TEXT( "AttackAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectSpecHandle AttackHandle = AbilitySystemComp->MakeOutgoingSpec( AttackAttribInitializer, GetCharacterLevel(), EffectContext );
	if( AttackHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *AttackHandle.Data.Get(), AbilitySystemComp.Get() );
	}

	if( !AgilityRefAttribInitializer )
	{
		RLOG( Error, TEXT( "AgilityRefAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectSpecHandle AGIHandle = AbilitySystemComp->MakeOutgoingSpec( AgilityRefAttribInitializer, GetCharacterLevel(), EffectContext );
	if( AGIHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *AGIHandle.Data.Get(), AbilitySystemComp.Get() );
	}
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
