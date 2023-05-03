// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGameMode.h"
#include "ActionRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AActionRPGGameMode::AActionRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
