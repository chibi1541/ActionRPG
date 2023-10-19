// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_SpendStamina.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "Interface/ARUsedTraceHitInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_SpendStamina)

UARGA_SpendStamina::UARGA_SpendStamina( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	StaminaCost = 0.f;

	ExhaustedTag = FGameplayTag::RequestGameplayTag( FName( "Gameplay.Character.State.Exhausted" ) );
	StaminaSpendTag = FGameplayTag::RequestGameplayTag( FName( "Gameplay.Character.State.SpendStamina" ) );

	ActivationBlockedTags.AddTag( ExhaustedTag );
	ActivationOwnedTags.AddTag( StaminaSpendTag );
}

void UARGA_SpendStamina::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
	}

	auto Interface = Cast<IARUsedTraceHitInterface>(this);

	bool bComplete = SpendStamina();

	if( bComplete == false )
	{
		RLOG( Error, TEXT( "GameplayEffect Implement is Failed" ) );
		EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
		return;
	}

	if( TriggerEventData )
	{
		CurrentEventData = *TriggerEventData;
	}

	if( bHasBlueprintActivate )
	{
		K2_ActivateAbility();
	}
	else if( bHasBlueprintActivateFromEvent )
	{
		if( TriggerEventData )
		{
			K2_ActivateAbilityFromEvent( *TriggerEventData );
		}
		else
		{
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled );
		}
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
					const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
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

void UARGA_SpendStamina::EndAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled )
{
	const auto Character = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() );
	if( Character )
	{
		AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if( AbilitySystemComponent.IsValid() )
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			if( StaminaRegenDelayEffect )
			{
				FGameplayEffectSpecHandle StaminaDelayHandle = AbilitySystemComponent->MakeOutgoingSpec( StaminaRegenDelayEffect, Character->GetCharacterLevel(), EffectContext );
				if( StaminaDelayHandle.IsValid() )
				{
					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *StaminaDelayHandle.Data.Get(), AbilitySystemComponent.Get() );
				}
			}
		}
	}

	Super::EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled );
}
