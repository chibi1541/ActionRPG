// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Actor.h"

#include "ARMinionSpawner.generated.h"

class ABaseMonster;

UENUM( BlueprintType )
enum class ESpawnerType :uint8
{
	ST_Minion		UMETA( DisplayName = "Minion" ),
	ST_Boss			UMETA( DisplayName = "Boss" ),
	Num				UMETA( Hidden )
};


UCLASS()
class ACTIONRPG_API AARMinionSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AARMinionSpawner();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable)
		bool SpawnMinion() const;
	
	const ESpawnerType GetSpawnerType() const;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion")
		TSubclassOf<ABaseMonster> MinionClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "SpawnerType" )
		ESpawnerType SpawnerType;
	
};
