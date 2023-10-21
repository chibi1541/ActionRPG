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

UENUM( BlueprintType )
enum class EAttribType : uint8
{
	AT_Base			UMETA( DisplayName = "BaseAttrib" ),
	AT_Health		UMETA( DisplayName = "HealthAttrib" ),
	AT_Attack		UMETA( DisplayName = "AttackAttrib" ),
	AT_Agility		UMETA( DisplayName = "AgilityAttrib" ),
	AT_Int			UMETA( DisplayName = "IntelligenceAttrib" ),
	Num				UMETA( Hidden )
};