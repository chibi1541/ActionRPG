// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/SceneComponent.h"

#include "ARTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UARTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARTargetComponent();

	UFUNCTION( BlueprintCallable )
	const FVector GetTargetLocation() const;
};
