// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFactor/ARMinionSpawnerManager.h"
#include "GameFactor/ARMinionSpawner.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AARMinionSpawnerManager::AARMinionSpawnerManager()
{
	PrimaryActorTick.bCanEverTick = false;

	Spawners.Empty();
	SpawnerIndex = 0;
}

void AARMinionSpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), AARMinionSpawner::StaticClass(), OutActors );

	for(auto Actor : OutActors )
	{
		if(auto Spawner = Cast<AARMinionSpawner>( Actor ))
		{
			Spawners.Add( Spawner );
		}
	}
}


void AARMinionSpawnerManager::SpawnMinion()
{
	
}

