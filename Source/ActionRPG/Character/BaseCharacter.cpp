// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AbilitySet.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/ARPGMovementComponent.h"
#include "Character/Attribute/ARBaseAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer.SetDefaultSubobjectClass<UARPGMovementComponent>( ACharacter::CharacterMovementComponentName ) )
{
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>( TEXT( "ARPGASC" ) );

	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	BaseAttribSet = CreateDefaultSubobject<UARBaseAttribSet>( TEXT( "ARBaseAttribSet" ) );
}

class UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return GetARPGAbilitySystemComponent();
}

void ABaseCharacter::GetOwnedGameplayTags( FGameplayTagContainer& TagContainer ) const
{
	AbilitySystemComponent->GetOwnedGameplayTags( TagContainer );
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( AbilitySystemComponent )
	{
		AbilitySystemComponent->InitAbilityActorInfo( this, this );
		InitAbilitySystem();

		InitializerAttributes();
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

void ABaseCharacter::InitializerAttributes()
{
	if( !BaseAttribInitializer )
	{
		RLOG(Error, TEXT( "BaseAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle BaseEffectContext = AbilitySystemComponent->MakeEffectContext();
	BaseEffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle BaseHandle = AbilitySystemComponent->MakeOutgoingSpec( BaseAttribInitializer, GetCharacterLevel(), BaseEffectContext );
	if( BaseHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *BaseHandle.Data.Get(), AbilitySystemComponent.Get() );
	}
}

int32 ABaseCharacter::GetCharacterLevel() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetCharacterLevel() );
	}

	return 0;
}

int32 ABaseCharacter::GetStrength() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetStrength() );
	}

	return 0;
}

int32 ABaseCharacter::GetAgility() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetAgility() );
	}

	return 0;
}

int32 ABaseCharacter::GetIntelligence() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetIntelligence() );
	}

	return 0;
}

int32 ABaseCharacter::GetVitality() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetVitality() );
	}

	return 0;
}
