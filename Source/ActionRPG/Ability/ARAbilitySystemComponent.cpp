// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARAbilitySystemComponent.h"
#include "Ability/ARGameplayAbility.h"
#include "Ability/GameplayEffect/ARGameplayEffect.h"

#include "Ability/TagRelationship.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARAbilitySystemComponent)

UE_DEFINE_GAMEPLAY_TAG( TAG_AbilityInputBlocked, "Gameplay.AbilityInputBlocked" );

UARAbilitySystemComponent::UARAbilitySystemComponent( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	ClearAbilityInput();

	GameplayTagsCallBacks.Empty();

	OnActiveGameplayEffectAddedDelegateToSelf.AddUObject( this, &UARAbilitySystemComponent::OnActiveGameEffect );
	OnAnyGameplayEffectRemovedDelegate().AddUObject( this, &UARAbilitySystemComponent::OnGameplayEffectRemoved );
}

void UARAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UARAbilitySystemComponent::AbilityInputTagPressed( const FGameplayTag& InputTag )
{
	if( InputTag.IsValid() )
	{
		for( const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				const UARGameplayAbility* AbilityCDO = CastChecked<UARGameplayAbility>( AbilitySpec.Ability );
				if( AbilityCDO->GetActivationPolicyType() == EARAbilityActivationPolicy::OnInputTriggered )
				{
					InputPressedSpecHandles.AddUnique( AbilitySpec.Handle );
				}
				else if( AbilityCDO->GetActivationPolicyType() == EARAbilityActivationPolicy::WhileInputHeld )
				{
					InputHeldSpecHandles.AddUnique( AbilitySpec.Handle );
				}
			}
		}
	}
}

void UARAbilitySystemComponent::AbilityInputTagReleased( const FGameplayTag& InputTag )
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

void UARAbilitySystemComponent::ProcessAbilityInput( float DeltaTime, bool bGamePaused )
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

void UARAbilitySystemComponent::SetTagRelationshipTable( UTagRelationship* NewTable )
{
	TagRelationshipTable = NewTable;
}

void UARAbilitySystemComponent::ApplyAbilityBlockAndCancelTags( const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags )
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if( TagRelationshipTable != nullptr )
	{
		TagRelationshipTable->GetAbilityTagsToBlockAndCancel( AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags );
	}

	Super::ApplyAbilityBlockAndCancelTags( AbilityTags, RequestingAbility, bEnableBlockTags, BlockTags, bExecuteCancelTags, CancelTags );
}

void UARAbilitySystemComponent::CancelAbilitiseWithTags( const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags )
{
	CancelAbilities( &WithTags, &WithoutTags );
}

void UARAbilitySystemComponent::OnTagUpdated( const FGameplayTag& Tag, bool TagExists )
{
}

void UARAbilitySystemComponent::OnActiveGameEffect( UAbilitySystemComponent* Owner, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle )
{
	const UARGameplayEffect* GameplayEffect = Cast<UARGameplayEffect>( EffectSpec.Def );
	if( !GameplayEffect || GameplayEffect->GetDelegateType() == EGameplayEffectDelegateType::EDT_None )
	{
		return;
	}

	if( FOnGameplayEffectCallbackDelegate* Delegate = ActiveGameplayEffectCallBacks.Find( GameplayEffect->GetDelegateType() ) )
	{
		RLOG( Warning, TEXT( "GameEffect Active, Name : %s" ), *GameplayEffect->GetFName().ToString() );
		Delegate->Broadcast( EffectSpec );
	}
}

void UARAbilitySystemComponent::OnGameplayEffectDurationChange( struct FActiveGameplayEffect& ActiveEffect )
{
	const UARGameplayEffect* GameplayEffect = Cast<UARGameplayEffect>( ActiveEffect.Spec.Def );
	if( !GameplayEffect || GameplayEffect->GetDelegateType() == EGameplayEffectDelegateType::EDT_None )
	{
		return;
	}

	if( FOnEffectDurationChangeCallbackDelegate* Delegate = GameplayEffectDurationChangeCallBacks.Find( GameplayEffect->GetDelegateType() ) )
	{
		RLOG( Warning, TEXT( "GameEffect Active, Name : %s" ), *GameplayEffect->GetFName().ToString() );
		Delegate->Broadcast( ActiveEffect );
	}
}

void UARAbilitySystemComponent::OnGameplayEffectRemoved( const FActiveGameplayEffect& ActiveEffect )
{
	const UARGameplayEffect* GameplayEffect = Cast<UARGameplayEffect>( ActiveEffect.Spec.Def );
	if( !GameplayEffect || GameplayEffect->GetDelegateType() == EGameplayEffectDelegateType::EDT_None )
	{
		return;
	}

	if( FOnGameplayEffectRemovedCallbackDelegate* Delegate = GameplayEffectRemoveCallBacks.Find( GameplayEffect->GetDelegateType() ) )
	{
		RLOG( Warning, TEXT( "GameEffect Active, Name : %s" ), *GameplayEffect->GetFName().ToString() );
		Delegate->Broadcast( ActiveEffect );
	}
}

void UARAbilitySystemComponent::GetAdditionalActivationTagRequirements( const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer& OutActivationRequired, OUT FGameplayTagContainer& OutActivationBlocked ) const
{
	if( TagRelationshipTable != nullptr )
	{
		TagRelationshipTable->GetRequiredAndBlockedActivationTags( AbilityTags, &OutActivationRequired, &OutActivationBlocked );
	}
}

bool UARAbilitySystemComponent::TryActivateAbilityByInputTag( const FGameplayTag& InputTag )
{
	bool Return = false;

	if( InputTag.IsValid() )
	{
		for( FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				if( AbilitySpec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor &&
					AbilitySpec.IsActive() )
				{
					Return = true;
				}
				else
				{
					Return = TryActivateAbility( AbilitySpec.Handle );
				}
			}
		}
	}

	return Return;
}

FGameplayAbilitySpec* UARAbilitySystemComponent::GetAbilitySpecByInputTag( const FGameplayTag& InputTag )
{
	if( InputTag.IsValid() )
	{
		for( FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items )
		{
			if( AbilitySpec.Ability != nullptr &&
				( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) ) )
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

