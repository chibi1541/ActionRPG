// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_UsingCooldown.h"

#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_UsingCooldown)

UARGA_UsingCooldown::UARGA_UsingCooldown( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	
}

void UARGA_UsingCooldown::ApplyCooldown( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if( CooldownGE )
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec( CooldownGE->GetClass(), GetAbilityLevel() );

		const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
		SpecHandle.Data.Get()->SetSetByCallerMagnitude( Tags.CooldownDurationTag, CooldownDuration.GetValueAtLevel( GetAbilityLevel() ) );
		ApplyGameplayEffectSpecToOwner( Handle, ActorInfo, ActivationInfo, SpecHandle );
	}
}
