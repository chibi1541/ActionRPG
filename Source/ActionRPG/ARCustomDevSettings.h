// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "ARCustomDevSettings.generated.h"

/**
 *
 */
UCLASS( config = Game, defaultconfig )
class ACTIONRPG_API UARCustomDevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UARCustomDevSettings( const FObjectInitializer& ObjectInitializer );

#if WITH_EDITORONLY_DATA

	// File Path for Im/Export
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Config, Category = "Data" )
	FString DataResourceFolder;

	// Auto Update Function On/Off
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Config, Category = "Data" )
	bool bAutoExportOnEdit;

#endif

	UFUNCTION( BlueprintPure )
	static UARCustomDevSettings* GetCustomDevSettings();

	virtual FName GetCategoryName() const override;


#if WITH_EDITOR

	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;


#endif // WITH_EDITOR

};
