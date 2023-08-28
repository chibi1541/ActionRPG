// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseMonster.h"

#include "Character/BaseAIController.h"
#include "Ability/ARPGAbilitySystemComponent.h"
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

	FGameplayEffectContextHandle HealthEffectContext = AbilitySystemComponent->MakeEffectContext();
	HealthEffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle HealthHandle = AbilitySystemComponent->MakeOutgoingSpec( HealthAttribInitializer, GetCharacterLevel(), HealthEffectContext );
	if( HealthHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *HealthHandle.Data.Get(), AbilitySystemComponent.Get() );
	}

	if( !AttackAttribInitializer )
	{
		RLOG( Error, TEXT( "AttackAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle AttackEffectContext = AbilitySystemComponent->MakeEffectContext();
	AttackEffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle AttackHandle = AbilitySystemComponent->MakeOutgoingSpec( AttackAttribInitializer, GetCharacterLevel(), AttackEffectContext );
	if( AttackHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *AttackHandle.Data.Get(), AbilitySystemComponent.Get() );
	}

	if( !AgilityRefAttribInitializer )
	{
		RLOG( Error, TEXT( "AgilityRefAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle AGIEffectContext = AbilitySystemComponent->MakeEffectContext();
	AGIEffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle AGIHandle = AbilitySystemComponent->MakeOutgoingSpec( AgilityRefAttribInitializer, GetCharacterLevel(), AGIEffectContext );
	if( AGIHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *AGIHandle.Data.Get(), AbilitySystemComponent.Get() );
	}
}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	SetHealth( GetMaxHealth() );
	SetStamina( GetMaxStamina() );
	SetShieldGauge( GetMaxShieldGauge() );

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
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
