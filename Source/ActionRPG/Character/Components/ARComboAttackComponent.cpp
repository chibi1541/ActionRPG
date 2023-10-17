// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARComboAttackComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARComboAttackComponent)

UARComboAttackComponent::UARComboAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurComboIndex = 0;
	bNextAttack = false;
	ComboAttackHandles.Reset();
}


// Called when the game starts
void UARComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Hero = Cast<ABaseCharacter>( GetOwner() );
	if( Hero )
	{
		AbilitySystemComponent = Hero->GetAbilitySystemComponent();
	}
}


void UARComboAttackComponent::SetNextAttack()
{
	if( CurComboIndex + 1 < ComboAttackHandles.Num() )
	{
		bNextAttack = true;
		return;
	}

	bNextAttack = false;
}


void UARComboAttackComponent::StartNextAttack( const FGameplayEventData* Payload )
{
	if( bNextAttack == false )
	{
		return;
	}

	if( CurComboIndex + 1 < ComboAttackHandles.Num() )
	{
		CurComboIndex++;
		bNextAttack = false;

		ActivateCurrentIndexAttack();
	}
}

void UARComboAttackComponent::ActivateCurrentIndexAttack()
{
	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "ASC is not Valid" ) );
		return;
	}

	bool IsActive = AbilitySystemComponent->TryActivateAbility( ComboAttackHandles[CurComboIndex] );

	if( IsActive &&
		CurComboIndex + 1 < ComboAttackHandles.Num() )
	{
		const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();
		DelegateHandle = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
			GameplayTags.AbilityStateTag_NextAttack ).AddUObject( this, &UARComboAttackComponent::StartNextAttack );
	}
}

void UARComboAttackComponent::RemoveDelegate()
{
	if( DelegateHandle.IsValid() && AbilitySystemComponent.IsValid() )
	{
		const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd( GameplayTags.AbilityStateTag_NextAttack ).Remove( DelegateHandle );
	}
}

void UARComboAttackComponent::StartComboAttack()
{
	if( NowAttacking() )
	{
		SetNextAttack();
		return;
	}

	if( ComboAttackHandles.Num() <= 0 )
	{
		RLOG( Warning, TEXT( "Do Not Have Combo Abilities" ) );
		return;
	}

	CurComboIndex = 0;
	bNextAttack = false;

	ActivateCurrentIndexAttack();
}

const bool UARComboAttackComponent::NowAttacking() const
{
	const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();
	if( AbilitySystemComponent.IsValid() &&
		AbilitySystemComponent->HasMatchingGameplayTag( GameplayTags.AbilityStateTag_Attack ) )
	{
		return true;
	}

	return false;
}

void UARComboAttackComponent::AddAttackAbilityHandle( const FGameplayAbilitySpecHandle& AttackHandle )
{
	ComboAttackHandles.Add( AttackHandle );
}
