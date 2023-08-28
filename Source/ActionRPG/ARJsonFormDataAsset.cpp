// Fill out your copyright notice in the Description page of Project Settings.


#include "ARJsonFormDataAsset.h"

#if WITH_EDITOR
#include "ARBlueprintFunctionLibrary.h"
#include "ARCustomDevSettings.h"
#include "UObject/UnrealType.h"
#include "Windows/WindowsPlatformProcess.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARJsonFormDataAsset)

#if	WITH_EDITOR

FString UARJsonFormDataAsset::FileExtension = TEXT( "json" );

void UARJsonFormDataAsset::Export()
{
	FString FilePath = UARBlueprintFunctionLibrary::GetExportFileName( this, FileExtension );
	UARBlueprintFunctionLibrary::ExportObjectAsJson( this, FilePath );
}

void UARJsonFormDataAsset::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
	if( PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd )
	{
		return;
	}

	const UARCustomDevSettings* Settings = UARCustomDevSettings::GetCustomDevSettings();
	if(Settings != nullptr && Settings->bAutoExportOnEdit)
	{
		Export();
	}
}

#endif

