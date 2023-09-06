// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GameplayEffect/ARGameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameplayEffect)

UARGameplayEffect::UARGameplayEffect( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	DelegateType = EGameplayEffectDelegateType::EDT_None;
}