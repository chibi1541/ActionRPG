// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_SpendStamina.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_SpendStamina)

UARGA_SpendStamina::UARGA_SpendStamina( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	StaminaCost = 0.f;

	ExhaustedTag = FGameplayTag::RequestGameplayTag( FName( "Gameplay.Character.State.Exhausted" ) );
	ActivationBlockedTags.AddTag( ExhaustedTag );
}

void UARGA_SpendStamina::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );

	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	bool bComplete = SpendStamina();

	if( bComplete == false )
	{
		RLOG( Error, TEXT( "GameplayEffect Implement is Failed" ) );
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}
}

bool UARGA_SpendStamina::SpendStamina()
{
	bool Return = false;
	AbilitySystemComponent.Reset();

	const auto Character = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() );
	if( Character )
	{
		AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if( AbilitySystemComponent.IsValid() )
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			if( StaminaSpandEffect )
			{
				FGameplayEffectSpecHandle StaminaCostHandle = AbilitySystemComponent->MakeOutgoingSpec( StaminaSpandEffect, Character->GetCharacterLevel(), EffectContext );
				if( StaminaCostHandle.IsValid() )
				{
					FActionRPGGlobalTags Tags = FActionRPGGlobalTags::Get();
					StaminaCostHandle.Data->SetSetByCallerMagnitude( Tags.AbilityCostTag_Stamina, StaminaCost );
					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *StaminaCostHandle.Data.Get(), AbilitySystemComponent.Get() );

					Return = true;
				}
			}
		}
	}

	return Return;
}

const bool UARGA_SpendStamina::IsExhausted() const
{
	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "AbilitySystemComponent is nullptr" ) );
		return true;
	}

	return AbilitySystemComponent->HasMatchingGameplayTag( ExhaustedTag );
}
