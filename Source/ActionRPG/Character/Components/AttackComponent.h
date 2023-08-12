// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"

#include "AttackComponent.generated.h"

class UAnimMontage;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent, DisplayName = "AttackComp" ) )
class ACTIONRPG_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack|Montages" )
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

public:
	FORCEINLINE TArray<TObjectPtr<UAnimMontage>> GetAttackMontages() const { return AttackMontages; }

};
