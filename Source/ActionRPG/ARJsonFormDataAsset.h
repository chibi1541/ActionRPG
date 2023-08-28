// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Engine/DataAsset.h"

#include "ARJsonFormDataAsset.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARJsonFormDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA

	static FString FileExtension;

#endif

#if WITH_EDITOR

	UFUNCTION( Category = "Import/Export", meta = ( CallInEditor = true ) )
	void Export();

	void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent );

#endif


};
