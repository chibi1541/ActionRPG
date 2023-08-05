// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARPGAbilitySystemComponent.h"
#include "Ability/ARPGGameplayAbility.h"

#include "Ability/TagRelationship.h"

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
				const UARPGGameplayAbility* AbilityCDO = CastChecked<UARPGGameplayAbility>( AbilitySpec.Ability );
				if( AbilityCDO->GetActivationPolicyType() == EARPGAbilityActivationPolicy::OnInputTriggered )
				{
					InputPressedSpecHandles.AddUnique( AbilitySpec.Handle );
				}
				else if( AbilityCDO->GetActivationPolicyType() == EARPGAbilityActivationPolicy::WhileInputHeld )
				{
					InputHeldSpecHandles.AddUnique( AbilitySpec.Handle );
				}
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

	for( const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles )
	{
		if( FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle( SpecHandle ) )
		{
			if( AbilitySpec->Ability != nullptr && AbilitySpec->IsActive() == false )
			{
				AbilitiesToActivate.AddUnique( AbilitySpec->Handle );
			}
		}
	}

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

void UARPGAbilitySystemComponent::SetTagRelationshipTable( UTagRelationship* NewTable )
{
	TagRelationshipTable = NewTable;
}

void UARPGAbilitySystemComponent::ApplyAbilityBlockAndCancelTags( const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags )
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if( TagRelationshipTable != nullptr )
	{
		TagRelationshipTable->GetAbilityTagsToBlockAndCancel( AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags );
	}

	Super::ApplyAbilityBlockAndCancelTags( AbilityTags, RequestingAbility, bEnableBlockTags, BlockTags, bExecuteCancelTags, CancelTags );
}

void UARPGAbilitySystemComponent::GetAdditionalActivationTagRequirements( const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer& OutActivationRequired, OUT FGameplayTagContainer& OutActivationBlocked ) const
{
	if( TagRelationshipTable != nullptr )
	{
		TagRelationshipTable->GetRequiredAndBlockedActivationTags( AbilityTags, &OutActivationRequired, &OutActivationBlocked );
	}
}

bool UARPGAbilitySystemComponent::TryActivateAbilityByInputTag( const FGameplayTag& InputTag )
{
	bool Return = false;

	if( InputTag.IsValid() )
	{
		for( const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				Return = TryActivateAbility( AbilitySpec.Handle );
				if( Return )
				{
					const UARPGGameplayAbility* AbilityCDO = CastChecked<UARPGGameplayAbility>( AbilitySpec.Ability );
					if( AbilityCDO->GetActivationPolicyType() == EARPGAbilityActivationPolicy::OnInputTriggered )
					{
						InputPressedSpecHandles.AddUnique( AbilitySpec.Handle );
					}
				}
			}
		}
	}

	return Return;
}

bool UARPGAbilitySystemComponent::GetAbilitySpecByInputTag( const FGameplayTag& InputTag, FGameplayAbilitySpec& OutSpec )
{
	if( InputTag.IsValid() )
	{
		for( FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				OutSpec = AbilitySpec;
				return true;
			}
		}
	}

	return false;
}

