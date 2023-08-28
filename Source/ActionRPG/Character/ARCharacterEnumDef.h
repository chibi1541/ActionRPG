// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM( BlueprintType )
enum class ECharacterType :uint8
{
	CT_Hero			UMETA( DisplayName = "Hero" ),
	CT_Monster		UMETA( DisplayName = "Monster" ),
	Num				UMETA( Hidden )
};

UENUM( BlueprintType )
enum class EHeroClass : uint8
{
	HC_Warrior		UMETA( DisplayName = "Warrior" ),
	HC_Wizard		UMETA( DisplayName = "Wizard" ),
	HC_Healer		UMETA( DisplayName = "Healer" ),
	Num				UMETA( Hidden )
};

UENUM( BlueprintType )
enum class EMonsterType : uint8
{
	MT_Proto		UMETA( DisplayName = "Proto" ),
	Num				UMETA( Hidden )
};