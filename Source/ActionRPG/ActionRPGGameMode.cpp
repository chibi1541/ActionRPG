// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameMode.h"
#include "Character/HeroCharacter.h"
#include "UObject/ConstructorHelpers.h"

AActionRPGGameMode::AActionRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> HeroBPClass( TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/B_HeroCharacter.B_HeroCharacter_C'" ) );
	if( HeroBPClass.Class != NULL )
	{
		DefaultPawnClass = HeroBPClass.Class;
	}

}
