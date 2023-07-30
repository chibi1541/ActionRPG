// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Character/BaseCharacter.h"

#include "BaseMonster.generated.h"

/**
 *
 */

UCLASS()
class ACTIONRPG_API ABaseMonster : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseMonster( const FObjectInitializer& ObjectInitializer );
};
