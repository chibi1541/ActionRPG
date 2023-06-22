// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/PlayerController.h"

#include "ARPGPlayerController.generated.h"

class UARPGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API AARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

private:
	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const;
};
