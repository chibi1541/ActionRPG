// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameplayMessage/AsyncAction_GameplayMessage.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/AssertionMacros.h"
#include "UObject/Class.h"
#include "UObject/Field.h"
#include "UObject/Script.h"
#include "UObject/ScriptMacros.h"
#include "UObject/Stack.h"
#include "UObject/UnrealType.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_GameplayMessage)

UAsyncAction_GameplayMessage* UAsyncAction_GameplayMessage::ListenForGameplayMessages( UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType )
{
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );
	if( !World )
	{
		return nullptr;
	}

	UAsyncAction_GameplayMessage* Action = NewObject<UAsyncAction_GameplayMessage>();
	Action->WorldPtr = World;
	Action->ChannelToRegister = Channel;
	Action->MessageStructType = PayloadType;
	Action->RegisterWithGameInstance( World );

	return Action;
}

bool UAsyncAction_GameplayMessage::GetPayload( int32& OutPayload )
{
	checkNoEntry();
	return false;
}

DEFINE_FUNCTION( UAsyncAction_GameplayMessage::execGetPayload )
{
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>( nullptr );
	void* MessagePtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>( Stack.MostRecentProperty );
	P_FINISH;

	bool bSuccess = false;

	// Make sure the type we are trying to get through the blueprint node matches the type of the message payload received.
	if( ( StructProp != nullptr ) && ( StructProp->Struct != nullptr ) && ( MessagePtr != nullptr ) && ( StructProp->Struct == P_THIS->MessageStructType.Get() ) && ( P_THIS->ReceivedMessagePayloadPtr != nullptr ) )
	{
		StructProp->Struct->CopyScriptStruct( MessagePtr, P_THIS->ReceivedMessagePayloadPtr );
		bSuccess = true;
	}

	*( bool* )RESULT_PARAM = bSuccess;
}


void UAsyncAction_GameplayMessage::Activate()
{
	if( UWorld* World = WorldPtr.Get() )
	{
		if( UARGameplayMessageSubsystem::HasInstance( World ) )
		{
			UARGameplayMessageSubsystem& Router = UARGameplayMessageSubsystem::Get( World );

			TWeakObjectPtr<UAsyncAction_GameplayMessage> WeakThis( this );
			ListenerHandle = Router.RegisterListenerInternal(
				ChannelToRegister,
				[WeakThis]( FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload )
				{
					if( UAsyncAction_GameplayMessage* StrongThis = WeakThis.Get() )
					{
						StrongThis->HandleMessageReceived( Channel, StructType, Payload );
					}
				},
				MessageStructType.Get() );

			return;
		}
	}

	SetReadyToDestroy();
}

void UAsyncAction_GameplayMessage::SetReadyToDestroy()
{
	ListenerHandle.Unregister();

	Super::SetReadyToDestroy();
}

void UAsyncAction_GameplayMessage::HandleMessageReceived( FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload )
{
	if( !MessageStructType.Get() || ( MessageStructType.Get() == StructType ) )
	{
		ReceivedMessagePayloadPtr = Payload;

		OnMessageReceived.Broadcast( this, Channel );

		ReceivedMessagePayloadPtr = nullptr;
	}

	if( !OnMessageReceived.IsBound() )
	{
		SetReadyToDestroy();
	}
}
