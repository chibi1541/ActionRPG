// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayTagContainer.h"
#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARGameState.generated.h"

#define MINION_CAPACITY 6
#define QUEST_KILL_COUNT 10

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

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:
	void AddMinionKillCount();

	UFUNCTION( BlueprintCallable )
		const int GetMinionKillCount() const;

	UFUNCTION( BlueprintCallable )
		const int GetQuestKillCount() const;

	UFUNCTION( BlueprintCallable )
		const float GetRemainTime() const;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Quest" )
		TSubclassOf<UUserWidget> QuestWidget;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Quest" )
		FGameplayTag QuestWidgetTag;

	FUIExtensionHandle QuestWidgetHandle;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Count" )
		TSubclassOf<UUserWidget> StartCountWidget;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Count" )
		FGameplayTag StartCountTag;

	FUIExtensionHandle CountWidgetHandle;

	UFUNCTION( BlueprintCallable )
		const bool GetPausedState() const;

	UFUNCTION( BlueprintCallable )
		void SetPausedState( bool bPause );

private:

	void CheckMinionTimer();

	void GenerateMinion();

	void GenerateBoss();

protected:

	UPROPERTY( Transient )
		int MinionKillCount;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Quest" )
		int QuestMinionCount;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Capacity" )
		int MinionCapacity;

	const float TIMERCOUNT = 5.f;

	float RemainTime;

	TArray<TWeakObjectPtr<AARMinionSpawner>> Spawners;

	TWeakObjectPtr<AARMinionSpawner> BossSpawner;

	int SpawnerIndex;

	bool bBossGene;

	FTimerHandle Timer;

	bool bGamePaused;

};
