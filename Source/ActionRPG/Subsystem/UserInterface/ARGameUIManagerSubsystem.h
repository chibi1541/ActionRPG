// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ARGameUIManagerSubsystem.generated.h"

class UARPrimaryGameLayout;
class ULocalPlayer;
class UCommonActivatableWidget;


/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	UARGameUIManagerSubsystem();

	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	TObjectPtr<UARPrimaryGameLayout> GetLayoutWidgetClass();

	void CreatePrimaryLayoutWidget( const ULocalPlayer* LocalPlayer );

	void AddMainGameWidget();

private:

	void AddLayoutToViewport( const ULocalPlayer* LocalPlayer, UARPrimaryGameLayout* Layout );

private:
	UPROPERTY( Transient )
		TObjectPtr<UARPrimaryGameLayout> PrimaryLayout;

	TArray<TWeakObjectPtr<UCommonActivatableWidget>> Layouts;
};
