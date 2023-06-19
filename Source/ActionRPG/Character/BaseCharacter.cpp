// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AbilitySet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>( TEXT( "ASC" ) );

	// 싱글플레이 용 설정
	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;
}

class UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( AbilitySystemComponent->IsValidLowLevel() == true )
	{
		RLOG_L( Warning );
		AbilitySystemComponent->InitAbilityActorInfo( this, this );
		InitAbilitySet();
	}
}

void ABaseCharacter::AddStartupGameplayAbilities()
{
	if( AbilitySystemComponent->IsValidLowLevel() == true )
	{
		for( TSubclassOf<UGameplayAbility>& startupAbility : GameplayAbilities )
		{
			RLOG_L( Warning );
			AbilitySystemComponent->GiveAbility( FGameplayAbilitySpec( startupAbility, 1, INDEX_NONE, this ) );
		}
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
