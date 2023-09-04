// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/PlayerController.h"

#include "ARPlayerController.generated.h"

class UARAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

private:
	UARAbilitySystemComponent* GetARAbilitySystemComponent() const;
};
