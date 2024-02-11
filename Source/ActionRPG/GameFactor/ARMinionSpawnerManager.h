// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARMinionSpawnerManager.generated.h"


class AARMinionSpawner;

UCLASS()
class ACTIONRPG_API AARMinionSpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AARMinionSpawnerManager();

protected:
	virtual void BeginPlay() override;


public:	
	void SpawnMinion();

protected:
	TArray<TObjectPtr<AARMinionSpawner>> Spawners;

	int SpawnerIndex;
};
