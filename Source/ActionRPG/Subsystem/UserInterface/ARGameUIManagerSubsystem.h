// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ARGameUIManagerSubsystem.generated.h"

class UARPrimaryGameLayout;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	TSubclassOf<UARPrimaryGameLayout> GetLayoutWidgetClass();

private:
	UPROPERTY( EditAnywhere )
		TSoftClassPtr<UARPrimaryGameLayout> LayoutClass;

};
