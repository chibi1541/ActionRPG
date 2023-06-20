// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "EnhancedInputComponent.h"
#include "Ability/HeroInputConfig.h"
#include "InputTriggers.h"

#include "HeroInputComponent.generated.h"

/**
 *
 */
UCLASS( Config = Input )
class ACTIONRPG_API UHeroInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	UHeroInputComponent( const FObjectInitializer& ObjectInitializer );

	template<class UserClass, typename FuncType>
	void BindNativeAction( const UHeroInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound );

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions( const UHeroInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc );
};

template<class UserClass, typename FuncType>
void UHeroInputComponent::BindNativeAction( const UHeroInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound )
{
	check( InputConfig );
	if( const UInputAction* IA = InputConfig->FindNativeInputActionForTag( InputTag, bLogIfNotFound ) )
	{	
		BindAction( IA, TriggerEvent, Object, Func );
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UHeroInputComponent::BindAbilityActions( const UHeroInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc )
{
	check( InputConfig );

	for( const FHeroInputAction Action : InputConfig->AbilityInputActions )
	{
		if( Action.InputAction != nullptr && Action.InputTag.IsValid() )
		{
			if( PressedFunc )
			{
				BindAction( Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag );
			}

			if( ReleasedFunc )
			{
				BindAction( Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag );
			}
		}
	}
}
