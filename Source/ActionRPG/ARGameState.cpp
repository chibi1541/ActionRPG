// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFactor/ARMinionSpawner.h"
#include "Character/BaseMonster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameState)


AARGameState::AARGameState( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	MinionKillCount = 0;
	SpawnerIndex = 0;

	RemainTime = TIMERCOUNT;

	Spawners.Empty();
	bBossGene = false;
}

void AARGameState::BeginPlay()
{
	
	TArray<AActor*> OutSpawners;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), AARMinionSpawner::StaticClass(), OutSpawners );

	for( auto OutSpawner : OutSpawners )
	{
		if( auto Spawner = Cast<AARMinionSpawner>( OutSpawner ) )
		{
			if( Spawner->GetSpawnerType() == ESpawnerType::ST_Minion )
			{
				Spawners.Add( Spawner );
			}
			else if( Spawner->GetSpawnerType() == ESpawnerType::ST_Boss )
			{
				BossSpawner = Spawner;
			}
		}
	}

	ShowCountUIWidget();

	GetWorldTimerManager().SetTimer( Timer, this, &AARGameState::CheckMinionTimer, 1.0f, true );

	if( UARGameUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UARGameUIManagerSubsystem>() )
	{
		QuestWidgetHandle = UIManager->RegisterExtensionWidget( QuestWidgetTag, QuestWidget, nullptr );
	}
}

void AARGameState::BeginDestroy()
{
	if( QuestWidgetHandle.IsValid() )
		QuestWidgetHandle.Unregister();

	Super::BeginDestroy();
}

void AARGameState::AddMinionKillCount()
{
	MinionKillCount++;

	if( MinionKillCount >= QUEST_KILL_COUNT )
		MinionKillCount = QUEST_KILL_COUNT;

	if( MinionKillCount >= QUEST_KILL_COUNT )
	{
		if( bBossGene == false )
		{
			GenerateBoss();

			bBossGene = true;
		}
	}
	else
	{
		GenerateMinion();
	}
}

const float AARGameState::GetRemainTime() const
{
	return RemainTime;
}

const int AARGameState::GetMinionKillCount() const
{
	return MinionKillCount;
}

const int AARGameState::GetQuestKillCount() const
{
	return QUEST_KILL_COUNT;
}

void AARGameState::CheckMinionTimer()
{
	RemainTime = RemainTime - GetWorldTimerManager().GetTimerElapsed(Timer);

	if( RemainTime <= 0.f )
	{
		GetWorldTimerManager().ClearTimer( Timer );

		for( int i = 0; i < MINION_CAPACITY; i++ )
		{
			GenerateMinion();
		}
	}
}

void AARGameState::GenerateMinion()
{
	if( SpawnerIndex >= Spawners.Num() )
	{
		SpawnerIndex = 0;
	}

	if( Spawners.Num() <= 0 )
	{
		RLOG( Warning, TEXT( "Spawner Not Found" ) );
		return;
	}

	Spawners[SpawnerIndex]->SpawnMinion();

	SpawnerIndex++;
}

void AARGameState::GenerateBoss()
{
	if( QuestWidgetHandle.IsValid() )
		QuestWidgetHandle.Unregister();

	BossSpawner->SpawnMinion();
}
