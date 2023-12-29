// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"

#include "ARHUDLayoutSet.generated.h"

class UARPrimaryGameLayout;

USTRUCT()
struct FHUDLayoutInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, Category = UI )
		TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY( EditAnywhere, Category = UI, meta = ( Categories = "UI.Layer" ) )
		FGameplayTag LayerTag;
};

USTRUCT()
struct FHUDElementInfo
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, Category = UI )
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

	UCommonActivatableWidget* AddLayout( UARPrimaryGameLayout* PrimaryLayout );



private:

	UPROPERTY( EditAnywhere, Category = UI )
		TArray<FHUDLayoutInfo> Layout;

	UPROPERTY( EditAnywhere, Category = UI )
		TArray<FHUDElementInfo> Widgets;

};
