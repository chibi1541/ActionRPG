// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCustomDevSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARCustomDevSettings)

#define LOCTEXT_NAMESPACE "CustomDevSettings"

UARCustomDevSettings::UARCustomDevSettings( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
#if WITH_EDITORONLY_DATA
	, DataResourceFolder( "Data" ), bAutoExportOnEdit( true )
#endif
{

}

UARCustomDevSettings* UARCustomDevSettings::GetCustomDevSettings()
{
	return GetMutableDefault<UARCustomDevSettings>();
}

FName UARCustomDevSettings::GetCategoryName() const
{
	return "Custom";
}

#if WITH_EDITOR

FText UARCustomDevSettings::GetSectionText() const
{
	return LOCTEXT( "UARCustomDevSettings::GetSectionText", "Import/Export" );
}

FText UARCustomDevSettings::GetSectionDescription() const
{
	return LOCTEXT( "UARCustomDevSettings::GetSectionDescription", "Configuring the export and import of data assets" );

}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE