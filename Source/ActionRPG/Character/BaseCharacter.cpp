// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AbilitySet.h"
#include "Ability/ARPGAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>( TEXT( "ASC" ) );

	// 싱글플레이 용 설정
	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;
}

class UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return GetARPGAbilitySystemComponent();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( AbilitySystemComponent->IsValidLowLevel() == true )
	{
		AbilitySystemComponent->InitAbilityActorInfo( this, this );
		InitAbilitySet();
	}
}

void ABaseCharacter::InitAbilitySet()
{
	for( const UAbilitySet* AbilitySet : AbilitySets )
	{
		if( AbilitySet != nullptr )
		{
			AbilitySet->GiveToAbilitySystem( AbilitySystemComponent );
		}
	}
}
