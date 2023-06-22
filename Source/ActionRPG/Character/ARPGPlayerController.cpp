// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ARPGPlayerController.h"

#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/HeroCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGPlayerController)

void AARPGPlayerController::PostProcessInput( const float DeltaTime, const bool bGamePaused )
{
	if(UARPGAbilitySystemComponent* ARpgASC = GetARPGAbilitySystemComponent())
	{
		ARpgASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UARPGAbilitySystemComponent* AARPGPlayerController::GetARPGAbilitySystemComponent() const
{
	const AHeroCharacter* HeroCharacter = CastChecked<AHeroCharacter>( GetCharacter() );
	return (HeroCharacter ? HeroCharacter->GetARPGAbilitySystemComponent() : nullptr );
}

