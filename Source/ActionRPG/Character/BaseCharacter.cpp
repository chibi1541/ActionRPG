// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AbilitySet.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/ARPGMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer.SetDefaultSubobjectClass<UARPGMovementComponent>( ACharacter::CharacterMovementComponentName ) )
{
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>( TEXT( "ASC" ) );

	// �̱��÷��� �� ����
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
		InitAbilitySystem();
	}
}

void ABaseCharacter::InitAbilitySystem()
{
	for( const UAbilitySet* AbilitySet : AbilitySets )
	{
		if( AbilitySet != nullptr )
		{
			AbilitySet->GiveToAbilitySystem( AbilitySystemComponent );
		}
	}
}
