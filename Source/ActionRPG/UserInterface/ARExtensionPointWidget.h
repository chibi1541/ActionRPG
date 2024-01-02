// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UObject/ObjectMacros.h"

#include "ARExtensionPointWidget.generated.h"

class ULocalPlayer;


UCLASS()
class ACTIONRPG_API UARExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam( TSubclassOf<UUserWidget>, FOnGetWidgetClassForData, UObject*, DataItem );
	DECLARE_DYNAMIC_DELEGATE_TwoParams( FOnConfigureWidgetForData, UUserWidget*, Widget, UObject*, DataItem );


	UARExtensionPointWidget( const FObjectInitializer& ObjectInitializer );


private:
	void ResetExtensionPoint();

};
