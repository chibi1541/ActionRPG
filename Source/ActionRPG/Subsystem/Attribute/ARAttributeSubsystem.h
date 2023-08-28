// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Character/ARCharacterEnumDef.h"

#include "ARAttributeSubsystem.generated.h"

USTRUCT( BlueprintType, Category = "Subsystem|Attrib" )
struct ACTIONRPG_API FCharacterCoefficientData
{
	GENERATED_BODY()

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


/**
 *
 */


UCLASS()
class ACTIONRPG_API UARAttributeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	virtual void Deinitialize() override;

private:
	UPROPERTY( BlueprintReadOnly, VisibleAnywhere, meta = ( AllowPrivateAccess = "true" ) )
	TMap<EHeroClass, FCharacterCoefficientData> HeroCoeffInfo;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere, meta = ( AllowPrivateAccess = "true" ) )
	TMap<EMonsterType, FCharacterCoefficientData> MonsterCoeffInfo;

public:

	const FCharacterCoefficientData* GetHeroCoeffData( EHeroClass HeroClass ) const;
	const FCharacterCoefficientData* GetMonsterCoeffData( EMonsterType MonsterType ) const;

};
