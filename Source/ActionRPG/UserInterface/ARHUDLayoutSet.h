// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
//#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARHUDLayoutSet.generated.h"

class UARPrimaryGameLayout;
class UARGameUIManagerSubsystem;

USTRUCT()
struct FHUDLayoutInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, Category = UI, meta = (AssetBundles = "Client" ))
		TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY( EditAnywhere, Category = UI, meta = ( Categories = "UI.Layer" ) )
		FGameplayTag LayerTag;
};

USTRUCT()
struct FHUDElementInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, Category = UI, meta = ( AssetBundles = "Client" ) )
		TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY( EditAnywhere, Category = UI )
		FGameplayTag SlotTag;
};

UCLASS( BlueprintType )
class ACTIONRPG_API UARHUDLayoutSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UARHUDLayoutSet();

private:

	friend UARGameUIManagerSubsystem;

	UPROPERTY( EditAnywhere, Category = UI )
		FHUDLayoutInfo Layout;

	UPROPERTY( EditAnywhere, Category = UI )
		TArray<FHUDElementInfo> Widgets;
};
