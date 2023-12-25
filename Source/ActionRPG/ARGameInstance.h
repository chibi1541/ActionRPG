// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/GameInstance.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "UserInterface/ARPrimaryGameLayout.h"

#include "ARGameInstance.generated.h"

class UARDA_AttributeInfo;

/**
 *
 */

UCLASS()
class ACTIONRPG_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY( EditAnywhere, Category = "Subsystem|Attribute" )
	TObjectPtr<UARDA_AttributeInfo> AttribSubsystemDataAsset;

	UPROPERTY( EditAnywhere, Category = "Subsystem|UIManager" )
	TSoftClassPtr<UARPrimaryGameLayout> PrimaryLayoutClass;

public:
	FORCEINLINE UARDA_AttributeInfo* GetAttribSubsystemDataAsset() const { return AttribSubsystemDataAsset; }

	FORCEINLINE TSubclassOf<UARPrimaryGameLayout> GetPrimaryLayoutClass() const { return PrimaryLayoutClass.LoadSynchronous(); }

};
