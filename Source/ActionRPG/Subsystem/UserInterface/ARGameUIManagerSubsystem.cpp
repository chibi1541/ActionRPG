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


void FUIExtensionHandle::Unregister()
{
	if( UARGameUIManagerSubsystem* Manager = UIManager.Get() )
	{
		Manager->UnregisterExtension( *this );
	}
}



//bool FUIExtensionPoint::DoesExtensionPassContract( const FUIExtension* Extension ) const
//{
//	if( UObject* DataPtr = Extension->WidgetClass )
//	{
//		const UClass* DataClass = DataPtr->IsA( UClass::StaticClass() ) ? Cast<UClass>( DataPtr ) : DataPtr->GetClass();
//		for( const UClass* AllowedDataClass : AllowedDataClasses )
//		{
//			if( DataClass->IsChildOf( AllowedDataClass ) || DataClass->ImplementsInterface( AllowedDataClass ) )
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}


UARGameUIManagerSubsystem::UARGameUIManagerSubsystem()
{

}

void UARGameUIManagerSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );
}

void UARGameUIManagerSubsystem::Deinitialize()
{



	Super::Deinitialize();
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

void UARGameUIManagerSubsystem::AddMainGameHUDWidget()
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
			if( TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = ( LayoutSet->Layout.LayoutClass.IsValid() ) ? LayoutSet->Layout.LayoutClass.Get() : LayoutSet->Layout.LayoutClass.LoadSynchronous() )
				Layouts.Add( PrimaryLayout->PushWidgetToLayerStack( LayoutSet->Layout.LayerTag, ConcreteWidgetClass ) );

			for( const FHUDElementInfo& Element : LayoutSet->Widgets )
			{
				RegisterExtensionWidget( Element.SlotTag, Element.WidgetClass.Get(), nullptr );
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

FUIExtensionHandle UARGameUIManagerSubsystem::RegisterExtensionWidget( const FGameplayTag& ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* Data )
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
	Entry->WidgetClass = WidgetClass;
	Entry->Data = Data;

	NotifyRegisterExtensionWidget( EExtensionAction::Added, Entry );

	return FUIExtensionHandle( this, Entry );
}

void UARGameUIManagerSubsystem::UnregisterExtensionPoint( const FGameplayTag& ExtensionPointTag )
{
	ExtensionPointMap.Remove( ExtensionPointTag );
}

void UARGameUIManagerSubsystem::UnregisterExtension( const FUIExtensionHandle& ExtensionHandle )
{
	if( ExtensionHandle.IsValid() )
	{
		checkf( ExtensionHandle.UIManager == this, TEXT( "Trying to unregister an extension that's not from this UIManager" ) );

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;
		if( FExtensionList* ListPtr = ExtensionMap.Find( Extension->ExtensionPointTag ) )
		{
			NotifyRegisterExtensionWidget( EExtensionAction::Removed, Extension );

			ListPtr->RemoveSwap( Extension );

			if( ListPtr->Num() == 0 )
			{
				ExtensionMap.Remove( Extension->ExtensionPointTag );
			}
		}
	}
	else
	{
		RLOG( Warning, TEXT( "Trying to unregister an invalid Handle." ) );
	}
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
				FUIExtensionRequest Request;
				Request.ExtensionHandle = FUIExtensionHandle( this, Extension );
				Request.ExtensionPointTag = Extension->ExtensionPointTag;
				Request.WidgetClass = Extension->WidgetClass;
				Request.Data = Extension->Data;
				ExtensionPoint->Callback.ExecuteIfBound( EExtensionAction::Added, Request );
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
				FUIExtensionRequest Request;
				Request.ExtensionHandle = FUIExtensionHandle( this, Extension );
				Request.ExtensionPointTag = Extension->ExtensionPointTag;
				Request.WidgetClass = Extension->WidgetClass;
				Request.Data = Extension->Data;
				ExtentionPoint->Get()->Callback.ExecuteIfBound( Action, Request );
			}
		}

		bOnInitialTag = false;
	}
}

