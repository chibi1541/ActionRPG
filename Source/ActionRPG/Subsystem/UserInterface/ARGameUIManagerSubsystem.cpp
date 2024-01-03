// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARGameInstance.h"
#include "UserInterface/ARPrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"
#include "UserInterface/ARHUDLayoutSet.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameUIManagerSubsystem)

void FUIExtensionPointHandle::Unregister()
{
	if( UARGameUIManagerSubsystem* Manager = UIManager.Get() )
	{
		Manager->UnregisterExtensionPoint( DataPtr->ExtensionPointTag );
	}
}


bool FUIExtensionPoint::DoesExtensionPassContract( const FUIExtension* Extension ) const
{
	if( UObject* DataPtr = Extension->Widget )
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

			for( const FHUDElementInfo& Element : LayoutSet->Widgets )
			{
				if( FExtensionList* ListPtr = ExtensionMap.Find( Element.SlotTag ) )
				{
					RegisterExtensionWidget( Element.SlotTag, Element.WidgetClass.Get() );
				}
			}
		}
	}


}

FUIExtensionPointHandle UARGameUIManagerSubsystem::RegisterExtensionPoint( const FGameplayTag& ExtensionPointTag, EExtensionPointMatch TagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback )
{
	if( !ExtensionPointTag.IsValid() )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension point." ) );
		return FUIExtensionPointHandle();
	}

	if( !ExtensionCallback.IsBound() )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension point : %s" ), *ExtensionPointTag.ToString() );
		return FUIExtensionPointHandle();
	}

	if( AllowedDataClasses.Num() == 0 )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension point : %s" ), *ExtensionPointTag.ToString() );
		return FUIExtensionPointHandle();
	}

	if( ExtensionPointMap.Contains( ExtensionPointTag ) )
	{
		RLOG( Error, TEXT( "Key Value's ExtensionPoint Already Exist : %s" ), *ExtensionPointTag.ToString() );
		return FUIExtensionPointHandle();
	}

	TSharedPtr<FUIExtensionPoint> Entry = MakeShared<FUIExtensionPoint>();
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->TagMatchType = TagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp( ExtensionCallback );

	ExtensionPointMap.Add( ExtensionPointTag, Entry );


	RLOG( Verbose, TEXT( "Extension Point [%s] Registered" ), *ExtensionPointTag.ToString() );

	NotifyRegisterExtensionPoint( Entry );

	return FUIExtensionPointHandle( this, Entry );
}

FUIExtensionHandle UARGameUIManagerSubsystem::RegisterExtensionWidget( const FGameplayTag& ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass )
{
	if( !ExtensionPointTag.IsValid() )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension : %s" ), *ExtensionPointTag.ToString() );
		return FUIExtensionHandle();
	}

	if( !WidgetClass )
	{
		RLOG( Error, TEXT( "Trying to register an invalid extension : %s" ), *ExtensionPointTag.ToString() );
		return FUIExtensionHandle();
	}

	FExtensionList& List = ExtensionMap.FindOrAdd( ExtensionPointTag );

	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef( MakeShared<FUIExtension>() );
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->Widget = WidgetClass;

	NotifyRegisterExtensionWidget( EExtensionAction::Added, Entry );

	return FUIExtensionHandle( this, Entry );
}

void UARGameUIManagerSubsystem::UnregisterExtensionPoint( const FGameplayTag& ExtensionPointTag )
{
	ExtensionPointMap.Remove( ExtensionPointTag );
}

void UARGameUIManagerSubsystem::NotifyRegisterExtensionPoint( TSharedPtr<FUIExtensionPoint>& ExtensionPoint )
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
					Request.ExtensionHandle = FUIExtensionHandle( this, Extension );
					Request.Widget = Extension->Widget;
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

void UARGameUIManagerSubsystem::NotifyRegisterExtensionWidget( EExtensionAction Action, TSharedPtr<FUIExtension>& Extension )
{
	bool bOnInitialTag = true;
	for( FGameplayTag Tag = Extension->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent() )
	{
		if( const TSharedPtr<FUIExtensionPoint>* ExtentionPoint = ExtensionPointMap.Find( Tag ) )
		{
			if( bOnInitialTag || ( ExtentionPoint->Get()->TagMatchType == EExtensionPointMatch::PartialMatch ) )
			{
				if( ExtentionPoint->Get()->DoesExtensionPassContract( Extension.Get() ) )
				{
					FUIExtensionRequest Request;
					Request.ExtensionHandle = FUIExtensionHandle( this, Extension );
					Request.ExtensionPointTag = Extension->ExtensionPointTag;
					Request.Widget = Extension->Widget;
					ExtentionPoint->Get()->Callback.ExecuteIfBound( Action, Request );
				}
			}
		}

		bOnInitialTag = false;
	}
}


