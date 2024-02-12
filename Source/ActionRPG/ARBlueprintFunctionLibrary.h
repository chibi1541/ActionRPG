// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ARBlueprintFunctionLibrary.generated.h"

class UDataTable;

/**
 *
 */


UCLASS()
class ACTIONRPG_API UARBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#if WITH_EDITOR

	UFUNCTION()
		static FString GetExportFileName( const UObject* Object, const FString& Extension );

	UFUNCTION()
		static void SaveStringToFile( const FString& String, const FString& FilePath );

	UFUNCTION()
		static void ExportDataTableAsCSV( const UDataTable* Table );

	UFUNCTION()
		static bool UObjectToJsonString( const UObject* Object, FString& OutJsonString );

	UFUNCTION()
		static void ExportObjectAsJson( const UObject* Object, const FString& OutExportPath );

#endif
};


