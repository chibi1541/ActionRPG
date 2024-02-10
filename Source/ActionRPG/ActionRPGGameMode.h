// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/GameModeBase.h"
#include "ActionRPGGameMode.generated.h"

class AARGameState;

UCLASS( minimalapi )
class AActionRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AActionRPGGameMode();

	virtual void PostInitializeComponents() override;

	void AddMinionKillCount();

protected:

	TWeakObjectPtr<AARGameState> GameStatePtr;

};



