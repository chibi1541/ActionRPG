// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARCharacterStateComponent.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Components/GetHitComponent.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARCharacterStateComponent)

// Sets default values for this component's properties
UARCharacterStateComponent::UARCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	StiffEffectSpecHandle = FGameplayEffectSpecHandle( nullptr );

	AbilitySystemComponent = CreateDefaultSubobject<UARAbilitySystemComponent>( TEXT( "ARASC" ) );
	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	IsStunned = false;
}

UAbilitySystemComponent* UARCharacterStateComponent::GetAbilitySystemComponent() const
{
	return GetARAbilitySystemComponent();
}

// Called when the game starts
void UARCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();
	if( !Owner )
	{
		RLOG( Error, TEXT( "Owner is not Valid" ) );
		return;
	}

	GetHitComp = CastChecked<UGetHitComponent>( Owner->GetComponentByClass( UGetHitComponent::StaticClass() ) );

	auto Tags = FActionRPGGlobalTags::Get();
	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( Tags.CharacterStateTag_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnGetStuned );
	AbilitySystemComponent->GameplayEffectRemoveCallBacks.FindOrAdd( Tags.CharacterStateTag_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnStunStateRemoved );
}

void UARCharacterStateComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	if( StiffEffectSpecHandle.IsValid() )
	{
		StiffEffectSpecHandle = FGameplayEffectSpecHandle( nullptr );
	}
}

void UARCharacterStateComponent::OnGetStuned( const FGameplayEffectSpec& EffectSpec )
{
	IsStunned = true;
}

void UARCharacterStateComponent::OnStunStateRemoved( const FActiveGameplayEffect& ActiveEffect )
{
	IsStunned = false;
}

void UARCharacterStateComponent::SetStiffEffectSpec( FGameplayEffectSpecHandle& SpecHandle )
{
	if( !SpecHandle.IsValid() )
	{
		RLOG( Error, TEXT( "Stiff SpecHandle is not Valid : %s" ), *GetOwner()->GetName() );
	}

	StiffEffectSpecHandle = SpecHandle;

	if( !GetHitComp.IsValid() )
	{
		RLOG( Error, TEXT( "Stiff Effect Activation is Failed : %s" ), *GetOwner()->GetName() );
		return;
	}

	float EffectDuration = GetHitComp->GetReactionMontagePlayTime( SpecHandle.Data->GetEffectContext().GetHitResult()->Normal );
	StiffEffectSpecHandle.Data->SetDuration( EffectDuration, false );

	FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *StiffEffectSpecHandle.Data.Get(), AbilitySystemComponent );
}

const bool UARCharacterStateComponent::GetStunState() const
{
	return IsStunned;
}
