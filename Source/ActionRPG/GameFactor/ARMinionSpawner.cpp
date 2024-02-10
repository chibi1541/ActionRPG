// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFactor/ARMinionSpawner.h"
#include "Character/BaseMonster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMinionSpawner)


AARMinionSpawner::AARMinionSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AARMinionSpawner::BeginPlay()
{
	Super::BeginPlay();
}

bool AARMinionSpawner::SpawnMinion() const
{
	if( MinionClass )
	{
		if( UWorld* World = GetWorld() )
		{
			ABaseMonster* Minion = World->SpawnActor<ABaseMonster>( MinionClass, GetActorTransform() );

			return ( Minion ) ? true : false;
		}
	}

	return false;
}

const ESpawnerType AARMinionSpawner::GetSpawnerType() const
{
	return SpawnerType;
}

