// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ComboAttackComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ComboAttackComponent)

UComboAttackComponent::UComboAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurComboIndex = 0;
	bNextAttack = false;
	ComboAttackHandles.Reset();
}


// Called when the game starts
void UComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Hero = Cast<ABaseCharacter>( GetOwner() );
	if( Hero )
	{
		AbilitySystemComponent = Hero->GetAbilitySystemComponent();
	}

}

void UComboAttackComponent::SetNextAttack()
{
	if( CurComboIndex + 1 < ComboAttackHandles.Num() )
	{
		bNextAttack = true;
		return;
	}

	bNextAttack = false;
}

void UComboAttackComponent::StartComboAttack()
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
	bNextAttack = false
		;
	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "ASC is not Valid" ) );
		return;
	}

	bool IsActive = AbilitySystemComponent->TryActivateAbility( ComboAttackHandles[CurComboIndex] );
	if( IsActive )
	{
		
	}
}

bool UComboAttackComponent::NowAttacking()
{
	const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();
	if( AbilitySystemComponent.IsValid() &&
		AbilitySystemComponent->HasMatchingGameplayTag( GameplayTags.AbilityStateTag_Attack ) )
	{
		return true;
	}

	return false;
}
