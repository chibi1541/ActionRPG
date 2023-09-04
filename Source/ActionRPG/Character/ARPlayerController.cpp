// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ARPlayerController.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/HeroCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPlayerController)

void AARPlayerController::PostProcessInput( const float DeltaTime, const bool bGamePaused )
{
	if( UARAbilitySystemComponent* ARASC = GetARAbilitySystemComponent())
	{
		ARASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UARAbilitySystemComponent* AARPlayerController::GetARAbilitySystemComponent() const
{
	const AHeroCharacter* HeroCharacter = CastChecked<AHeroCharacter>( GetCharacter() );
	return (HeroCharacter ? HeroCharacter->GetARAbilitySystemComponent() : nullptr );
}

