// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameMode.h"

#include "Character/HeroCharacter.h"
#include "Character/ARPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AActionRPGGameMode::AActionRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> HeroBPClass( TEXT( "/Game/Blueprints/B_HeroCharacter" ) );
	if( HeroBPClass.Class != NULL )
	{
		DefaultPawnClass = HeroBPClass.Class;
	}

	PlayerControllerClass = AARPlayerController::StaticClass();
}
