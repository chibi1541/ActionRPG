// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBlueprintFunctionLibrary.h"
#include "ARCustomDevSettings.h"
#include "Engine/DataTable.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "SourceControlHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARBlueprintFunctionLibrary)

#if WITH_EDITOR

FString UARBlueprintFunctionLibrary::GetExportFileName( const UObject* Object, const FString& Extension )
{
	static const FString PathPrefix = TEXT( "/Game" );
	const UARCustomDevSettings* Settings = UARCustomDevSettings::GetCustomDevSettings();
	if( Settings != nullptr )
	{
		const FString ProjectDir = FPaths::ProjectDir();
		FString FileName = Object->GetPackage()->GetFName().ToString() + TEXT( "." ) + Extension;
		if( FileName.StartsWith( PathPrefix ) )
		{
			FileName = FileName.RightChop( PathPrefix.Len() );
		}
		const FString OutputPath = FPaths::Combine( ProjectDir, Settings->DataResourceFolder, FileName );
		return OutputPath;
	}

	return FString();
}

void UARBlueprintFunctionLibrary::SaveStringToFile( const FString& String, const FString& FilePath )
{
	if( USourceControlHelpers::IsEnabled() )
	{
		USourceControlHelpers::CheckOutFile( FilePath );
	}

	FFileHelper::SaveStringToFile( String, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8 );
	if( USourceControlHelpers::IsEnabled() )
	{
		USourceControlHelpers::MarkFileForAdd( FilePath );
	}
}

void UARBlueprintFunctionLibrary::ExportDataTableAsCSV( const UDataTable* Table )
{
	if( Table == nullptr )
	{
		RLOG( Error, TEXT( "Exporting is Failed. UDataTable is Null" ) );
		return;
	}

	const FString CSVString = Table->GetTableAsCSV( EDataTableExportFlags::None );
	const FString ExportPath = GetExportFileName( Table, TEXT( "csv" ) );

	SaveStringToFile( CSVString, ExportPath );
}

bool UARBlueprintFunctionLibrary::UObjectToJsonString( const UObject* Object, FString& OutJsonString )
{
	// UObject to Json Character 
	TSharedPtr<FJsonObject> JsonObject = MakeShareable( new FJsonObject() );
	FJsonObjectConverter::UStructToJsonObject( Object->GetClass(), ( void* )( Object ), JsonObject.ToSharedRef(), 0, 0, nullptr );
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter =
		TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create( &OutJsonString, 0 );

	return FJsonSerializer::Serialize( JsonObject.ToSharedRef(), JsonWriter );
}

void UARBlueprintFunctionLibrary::ExportObjectAsJson( const UObject* Object, const FString& OutExportPath )
{
	if( !Object )
	{
		return;
	}

	FString JsonString;
	if( UObjectToJsonString( Object, OUT JsonString ) )
	{
		SaveStringToFile( JsonString, OutExportPath );
	}
	else
	{
		RLOG( Error, TEXT( "UARBlueprintFunctionLibrary::ExportObjectAsJson: Failed to Export, %s" ), *Object->GetName() );
	}
}

#endif
