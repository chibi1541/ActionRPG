// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ARMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UARMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Sprint")
	void StartSprint();

	UFUNCTION( BlueprintCallable, Category = "Ability|Sprint" )
	void EndSprint();


protected:
	uint8 bIsSprinting : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Sprint", meta = (AllowPrivateAccess = "true" ))
	float SprintingSpeedRate;
};
