// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/GameStateBase.h"

#include "ARGameState.generated.h"

#define QUEST_MINION_COUNT 30

/**
 *
 */
UCLASS()
class ACTIONRPG_API AARGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AARGameState( const FObjectInitializer& ObjectInitializer );

public:
	void AddMinionKillCount();

	const int GetMinionKillCount() const;


private:

	UPROPERTY( Transient )
		int MinionKillCount;


};
