// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARGameInstance.h"
#include "UserInterface/ARPrimaryGameLayout.h"
#include "Engine/LocalPlayer.h"
#include "UserInterface/ARHUDLayoutSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameUIManagerSubsystem)



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

			AddLayoutToViewport( LocalPlayer, PrimaryLayout );
		}
	}
}

void UARGameUIManagerSubsystem::AddLayoutToViewport( const ULocalPlayer* LocalPlayer, UARPrimaryGameLayout* Layout )
{
	UE_LOG( ActionRPG, Log, TEXT( "[%s] is adding player [%s]'s root layout [%s] to the viewport" ), *GetName(), *GetNameSafe( LocalPlayer ), *GetNameSafe( Layout ) );

	Layout->SetPlayerContext( FLocalPlayerContext( LocalPlayer ) );
	Layout->AddToPlayerScreen( 1000 );
}

void UARGameUIManagerSubsystem::AddMainGameWidget()
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
			Layouts.Add( LayoutSet->AddLayout( PrimaryLayout ) );
		}
	}
}
