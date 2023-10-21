// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayEffectSet.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayEffectSet)

void UGameplayEffectSet::AttribInitialize( UAbilitySystemComponent* AbilitySystemComponent ) const
{
	RCHECK( AbilitySystemComponent != nullptr );

	RCHECK( BaseInitializer != nullptr );

	bool IsBaseAttribInit = false;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle BaseEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec( BaseInitializer, 1, EffectContext );
	if( BaseEffectSpecHandle.IsValid() )
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *BaseEffectSpecHandle.Data.Get(), AbilitySystemComponent );
		IsBaseAttribInit = true;
	}

	if( IsBaseAttribInit )
	{
		for( auto Effect : AttribInitializers )
		{
			FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec( Effect, 1, EffectContext );
			if( EffectSpecHandle.IsValid() )
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *EffectSpecHandle.Data.Get(), AbilitySystemComponent );
			}
		}
	}
}

void UGameplayEffectSet::PassiveInitialize( UAbilitySystemComponent* AbilitySystemComponent ) const
{
	RCHECK( AbilitySystemComponent != nullptr );

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

	for( auto Effect : PassiveEffects )
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec( Effect, 1, EffectContext );
		if( NewHandle.IsValid() )
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *NewHandle.Data.Get(), AbilitySystemComponent );
		}
	}
}
