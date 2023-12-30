// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ARPlayerController.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/HeroCharacter.h"
#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPlayerController)

void AARPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// Create Primary Widget
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
	if( LocalPlayer )
	{
		if( UARGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UARGameUIManagerSubsystem>() )
		{
			UIManager->CreatePrimaryLayoutWidget( LocalPlayer );
			UIManager->AddMainGameLayoutWidget();
		}
	}
}

void AARPlayerController::PostProcessInput( const float DeltaTime, const bool bGamePaused )
{
	if( UARAbilitySystemComponent* ARASC = GetARAbilitySystemComponent() )
	{
		ARASC->ProcessAbilityInput( DeltaTime, bGamePaused );
	}

	Super::PostProcessInput( DeltaTime, bGamePaused );

	bShouldPerformFullTickWhenPaused = true;
}

UARAbilitySystemComponent* AARPlayerController::GetARAbilitySystemComponent() const
{
	const AHeroCharacter* HeroCharacter = CastChecked<AHeroCharacter>( GetCharacter() );
	return ( HeroCharacter ? HeroCharacter->GetARAbilitySystemComponent() : nullptr );
}

