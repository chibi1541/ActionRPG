// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"

#include "ARPrimaryGameLayout.generated.h"

/**
 *
 */
UCLASS( Abstract, meta = ( DisableNativeTick ) )
class ACTIONRPG_API UARPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()


public:
	UARPrimaryGameLayout( const FObjectInitializer& ObjectInitializer );

	UCommonActivatableWidgetContainerBase* GetLayerWidget( const FGameplayTag& LayerTag ) const;

protected:
	UFUNCTION( BlueprintCallable, Category = "Layer" )
		void RegisterLayer( UPARAM( meta = ( Categories = "UI.Layer" ) ) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget );

private:
	UPROPERTY( Transient, meta = ( Categories = "UI.Layer" ) )
		TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
