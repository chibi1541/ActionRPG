// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Components/ARCharacterStateComponent.h"

#include "SubHeroStatusMessage.generated.h"
/**
 * 
 */
USTRUCT( BlueprintType )
struct FSubHeroStatusMessage
{
	GENERATED_BODY()

public:
	UPROPERTY( BlueprintReadWrite )
		TObjectPtr<UARCharacterStateComponent> StateComponent = nullptr;


};
