// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Ability/AbilitySet.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/ARPGMovementComponent.h"
#include "Character/Attribute/ARPGBaseAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer.SetDefaultSubobjectClass<UARPGMovementComponent>( ACharacter::CharacterMovementComponentName ) )
{
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>( TEXT( "ARPGASC" ) );

	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	BaseAttributeSet = CreateDefaultSubobject<UARPGBaseAttributeSet>( TEXT( "ARPGBaseAttributeSet" ) );
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
	if( !AttributeInitializer )
	{
		RLOG(Error, TEXT( "AttributeInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec( AttributeInitializer, GetCharacterLevel(), EffectContext );
	if( NewHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *NewHandle.Data.Get(), AbilitySystemComponent.Get() );
	}
}

int32 ABaseCharacter::GetCharacterLevel() const
{
	if( BaseAttributeSet )
	{
		return static_cast< int32 >( BaseAttributeSet->GetCharacterLevel() );
	}

	return 0;
}

int32 ABaseCharacter::GetStrength() const
{
	if( BaseAttributeSet )
	{
		return static_cast< int32 >( BaseAttributeSet->GetStrength() );
	}

	return 0;
}

int32 ABaseCharacter::GetAgility() const
{
	if( BaseAttributeSet )
	{
		return static_cast< int32 >( BaseAttributeSet->GetAgility() );
	}

	return 0;
}

int32 ABaseCharacter::GetIntelligence() const
{
	if( BaseAttributeSet )
	{
		return static_cast< int32 >( BaseAttributeSet->GetIntelligence() );
	}

	return 0;
}

int32 ABaseCharacter::GetVitality() const
{
	if( BaseAttributeSet )
	{
		return static_cast< int32 >( BaseAttributeSet->GetVitality() );
	}

	return 0;
}
