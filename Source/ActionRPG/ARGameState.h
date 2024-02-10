// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/GameStateBase.h"

#include "ARGameState.generated.h"

#define MINION_CAPACITY 6
#define QUEST_KILL_COUNT 2

class AARMinionSpawner;


/**
 *
 */
UCLASS()
class ACTIONRPG_API AARGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AARGameState( const FObjectInitializer& ObjectInitializer );

	virtual void PostInitializeComponents() override;

public:
	void AddMinionKillCount();

	UFUNCTION( BlueprintCallable )
		const int GetMinionKillCount() const;

	UFUNCTION( BlueprintCallable )
		const int GetQuestKillCount() const;

private:

	void GenerateMinion();

	void GenerateBoss();

protected:

	UPROPERTY( Transient )
		int MinionKillCount;

	TArray<TWeakObjectPtr<AARMinionSpawner>> Spawners;

	TWeakObjectPtr<AARMinionSpawner> BossSpawner;

	int SpawnerIndex;

	bool bBossGene;

};
