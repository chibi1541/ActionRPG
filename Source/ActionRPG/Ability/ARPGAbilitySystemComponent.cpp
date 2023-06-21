// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARPGAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGAbilitySystemComponent)

UARPGAbilitySystemComponent::UARPGAbilitySystemComponent( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UARPGAbilitySystemComponent::AbilityInputTagPressed( const FGameplayTag& InputTag )
{
	if( InputTag.IsValid() )
	{
		for( const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability && ( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				InputPressedSpecHandles.AddUnique( AbilitySpec.Handle );
				InputHeldSpecHandles.AddUnique( AbilitySpec.Handle );
			}
		}
	}
}

void UARPGAbilitySystemComponent::AbilityInputTagReleased( const FGameplayTag& InputTag )
{
	if( InputTag.IsValid() )
	{
		for( const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability && ( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				InputReleasedSpecHandles.AddUnique( AbilitySpec.Handle );
				InputHeldSpecHandles.Remove( AbilitySpec.Handle );
			}
		}
	}
}

