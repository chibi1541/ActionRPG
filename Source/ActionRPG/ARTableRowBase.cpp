// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTableRowBase.h"

#if WITH_EDITOR
#include "ARCustomDevSettings.h"
#include "ARBlueprintFunctionLibrary.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARTableRowBase)

#if WITH_EDITOR
void FARTableRowBase::OnDataTableChanged( const UDataTable* InDataTable, const FName InRowName )
{
	const UARCustomDevSettings* Settings = UARCustomDevSettings::GetCustomDevSettings();
	if( ( Settings != nullptr ) && ( Settings->bAutoExportOnEdit ) )
	{
		UARBlueprintFunctionLibrary::ExportDataTableAsCSV( InDataTable );
	}
}
#endif
