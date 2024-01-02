// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARGameInstance.h"
#include "UserInterface/ARPrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"
#include "UserInterface/ARHUDLayoutSet.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameUIManagerSubsystem)


bool FUIExtensionPoint::DoesExtensionPassContract( const FUIExtension* Extension ) const
{
	if( UObject* DataPtr = Extension->Data )
	{
		const bool bMatchesContext =
			( ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull() ) ||
			ContextObject == Extension->ContextObject;

		if( bMatchesContext )
		{
			const UClass* DataClass = DataPtr->IsA( UClass::StaticClass() ) ? Cast<UClass>( DataPtr ) : DataPtr->GetClass();
			for( const UClass* AllowedDataClass : AllowedDataClasses )
			{
				if( DataClass->IsChildOf( AllowedDataClass ) || DataClass->ImplementsInterface( AllowedDataClass ) )
				{
					return true;
				}
			}
		}
	}

	return false;
}


UARGameUIManagerSubsystem::UARGameUIManagerSubsystem()
{

}

void UARGameUIManagerSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );
}

TObjectPtr<UARPrimaryGameLayout> UARGameUIManagerSubsystem::GetLayoutWidgetClass()
{
	return PrimaryLayout;
}

void UARGameUIManagerSubsystem::CreatePrimaryLayoutWidget( const ULocalPlayer* LocalPlayer )
{
	const UARGameInstance* GameInstance = Cast<UARGameInstance>( GetGameInstance() );
	check( GameInstance );

	TSubclassOf<UARPrimaryGameLayout> PrimaryLayoutClass = GameInstance->GetPrimaryLayoutClass();

	if( APlayerController* PlayerController = LocalPlayer->GetPlayerController( GetWorld() ) )
	{
		if( ensure( PrimaryLayoutClass && !PrimaryLayoutClass->HasAnyClassFlags( CLASS_Abstract ) ) )
		{
			PrimaryLayout = CreateWidget<UARPrimaryGameLayout>( PlayerController, PrimaryLayoutClass );

			if( PrimaryLayout )
			{
				UE_LOG( ActionRPG, Log, TEXT( "[%s] is adding player [%s]'s root layout [%s] to the viewport" ), *GetName(), *GetNameSafe( LocalPlayer ), *GetNameSafe( PrimaryLayout ) );

				PrimaryLayout->SetPlayerContext( FLocalPlayerContext( LocalPlayer ) );
				PrimaryLayout->AddToPlayerScreen( 1000 );
			}
		}
	}
}

void UARGameUIManagerSubsystem::AddMainGameLayoutWidget()
{
	if( !PrimaryLayout )
	{
		// error log 
		return;
	}


	if( ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( PrimaryLayout->GetOwningLocalPlayer() ) )
	{
		const UARGameInstance* GameInstance = Cast<UARGameInstance>( GetGameInstance() );
		check( GameInstance );

		if( TObjectPtr<UARHUDLayoutSet> LayoutSet = GameInstance->GetMainGameHUDLayoutSet() )
		{
			Layouts.Add( PrimaryLayout->PushWidgetToLayerStack( LayoutSet->Layout.LayerTag, LayoutSet->Layout.LayoutClass.Get() ) );
		}
	}
}
 
FUIExtensionPointHandle UARGameUIManagerSubsystem::RegisterExtensionPoint( const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EExtensionPointMatch TagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback )
{
	if( !ExtensionPointTag.IsValid() )
	{
		RLOG(Error, TEXT( "Trying to register an invalid extension point." ) );
		return FUIExtensionPointHandle();
	}

	if( !ExtensionCallback.IsBound() )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension point." ) );
		return FUIExtensionPointHandle();
	}

	if( AllowedDataClasses.Num() == 0 )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension point." ) );
		return FUIExtensionPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd( ExtensionPointTag );

	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef( MakeShared<FUIExtensionPoint>() );
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->TagMatchType = TagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp( ExtensionCallback );

	RLOG( Verbose, TEXT( "Extension Point [%s] Registered" ), *ExtensionPointTag.ToString() );

	NotifyExtensionPointOfExtensions( Entry );

	return FUIExtensionPointHandle( Entry );
}

void UARGameUIManagerSubsystem::NotifyExtensionPointOfExtensions( TSharedPtr<FUIExtensionPoint>& ExtensionPoint )
{
	for( FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent() )
	{
		if( const FExtensionList* ListPtr = ExtensionMap.Find( Tag ) )
		{
			// Copy in case there are removals while handling callbacks
			FExtensionList ExtensionArray( *ListPtr );

			for( const TSharedPtr<FUIExtension>& Extension : ExtensionArray )
			{
				if( ExtensionPoint->DoesExtensionPassContract( Extension.Get() ) )
				{
					FUIExtensionRequest Request;
					Request.ExtensionHandle = FUIExtensionHandle( Extension );
					Request.Data = Extension->Data;
					ExtensionPoint->Callback.ExecuteIfBound( EExtensionAction::Added, Request );
				}
			}
		}

		if( ExtensionPoint->TagMatchType == EExtensionPointMatch::ExactMatch )
		{
			break;
		}
	}
}