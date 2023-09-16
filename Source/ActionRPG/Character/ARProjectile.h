// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"

#include "ARProjectile.generated.h"

UCLASS()
class ACTIONRPG_API AARProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Meta = ( ExposeOnSpawn = true ) )
		float Range;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Meta = ( ExposeOnSpawn = true ) )
		float Speed;

	UPROPERTY( BlueprintReadOnly, Meta = ( ExposeOnSpawn = true ) )
		FGameplayEffectSpecHandle DamageCalcEffectSpecHandle;

	UPROPERTY( BlueprintReadOnly, Meta = ( ExposeOnSpawn = true ) )
		FGameplayEffectSpecHandle StiffEffectSpecHandle;

protected:

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
		class UProjectileMovementComponent* ProjectileMovementComp;



};
