// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/HeroInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroInputConfig)

UHeroInputConfig::UHeroInputConfig( const FObjectInitializer& Objectinitializer )
{
}

const UInputAction* UHeroInputConfig::FindNativeInputActionForTag( const FGameplayTag& InputTag, bool bLogNotFound ) const
{
	for( const FHeroInputAction& Action : NativeInputActions )
	{
		if( Action.InputAction != nullptr && Action.InputTag == InputTag )
		{
			return Action.InputAction;
		}
	}

	if( bLogNotFound == true )
	{
		RLOG( Error, TEXT( "Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]." ), *InputTag.ToString(), *GetNameSafe( this ) );
	}

	return nullptr;
}

const UInputAction* UHeroInputConfig::FindAbilityInputActionForTag( const FGameplayTag& InputTag, bool bLogNotFound ) const
{
	for( const FHeroInputAction& Action : AbilityInputActions )
	{
		if( Action.InputAction != nullptr && Action.InputTag == InputTag )
		{
			return Action.InputAction;
		}
	}

	if( bLogNotFound == true )
	{
		RLOG( Error, TEXT( "Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]." ), *InputTag.ToString(), *GetNameSafe( this ) );
	}

	return nullptr;
}
