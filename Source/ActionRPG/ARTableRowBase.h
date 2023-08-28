// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ARTableRowBase.generated.h"

USTRUCT()
struct ACTIONRPG_API FARTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:


#if WITH_EDITOR
	virtual void OnDataTableChanged( const UDataTable* InDataTable, const FName InRowName ) override;

#endif
};

USTRUCT()
struct ACTIONRPG_API FARCharCoeffTableData : public FARTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float AttackDamageCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float HealthCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float ManaCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float StaminaCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float DefenceCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float ShieldGaugeCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float MoveSpeedCoeff;

	UPROPERTY( EditAnywhere, Category = "Coefficient" )
	float AttackSpeedCoeff;

};