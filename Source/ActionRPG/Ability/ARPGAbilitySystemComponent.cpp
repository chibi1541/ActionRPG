// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARPGAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGAbilitySystemComponent)

UE_DEFINE_GAMEPLAY_TAG( TAG_AbilityInputBlocked, "Gameplay.AbilityInputBlocked" );

UARPGAbilitySystemComponent::UARPGAbilitySystemComponent( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UARPGAbilitySystemComponent::ClearAbilityInput()
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
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
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
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				InputReleasedSpecHandles.AddUnique( AbilitySpec.Handle );
				InputHeldSpecHandles.Remove( AbilitySpec.Handle );
			}
		}
	}
}

void UARPGAbilitySystemComponent::ProcessAbilityInput( float DeltaTime, bool bGamePaused )
{
	if( HasMatchingGameplayTag( TAG_AbilityInputBlocked ) )
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for( const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles )
	{
		if( FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if( AbilitySpec->Ability != nullptr )
			{
				AbilitySpec->InputPressed = true;

				if( AbilitySpec->IsActive() )
				{
					AbilitySpecInputPressed( *AbilitySpec );
				}
				else
				{
					AbilitiesToActivate.AddUnique( SpecHandle );
				}
			}
		}
	}

	for( const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate )
	{
		TryActivateAbility( AbilitySpecHandle );
	}

	for( const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles )
	{
		if( FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if( AbilitySpec->Ability != nullptr )
			{
				AbilitySpec->InputPressed = false;

				if( AbilitySpec->IsActive() )
				{
					AbilitySpecInputReleased( *AbilitySpec );
				}
			}

		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

