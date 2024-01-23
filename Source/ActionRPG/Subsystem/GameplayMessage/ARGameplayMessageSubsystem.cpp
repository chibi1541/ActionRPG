// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameplayMessage/ARGameplayMessageSubsystem.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameplayMessageSubsystem)

DEFINE_LOG_CATEGORY( LogGameplayMessageSubsystem );


namespace UE
{
	namespace GameplayMessageSubsystem
	{
		static int32 ShouldLogMessages = 0;
		static FAutoConsoleVariableRef CVarShouldLogMessages(
			TEXT( "GameplayMessageSubsystem.LogMessages" ),
			ShouldLogMessages,
			TEXT( "Should messages broadcast through the gameplay message subsystem be logged?" ) );
	}
}

void FGameplayMessageListenerHandle::Unregister()
{
	if( UARGameplayMessageSubsystem* StrongSubsystem = Subsystem.Get() )
	{
		StrongSubsystem->UnregisterListener( *this );
		Subsystem.Reset();
		Channel = FGameplayTag();
		ID = 0;
	}
}

UARGameplayMessageSubsystem& UARGameplayMessageSubsystem::Get( const UObject* WorldContextObject )
{
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::Assert );
	check( World );
	UARGameplayMessageSubsystem* Router = UGameInstance::GetSubsystem<UARGameplayMessageSubsystem>( World->GetGameInstance() );
	check( Router );
	return *Router;
}

bool UARGameplayMessageSubsystem::HasInstance( const UObject* WorldContextObject )
{
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::Assert );
	UARGameplayMessageSubsystem* Router = World != nullptr ? UGameInstance::GetSubsystem<UARGameplayMessageSubsystem>( World->GetGameInstance() ) : nullptr;
	return Router != nullptr;
}

void UARGameplayMessageSubsystem::Deinitialize()
{
	ListenerMap.Reset();

	Super::Deinitialize();
}

void UARGameplayMessageSubsystem::UnregisterListener( FGameplayMessageListenerHandle Handle )
{
	if( Handle.IsValid() )
	{
		check( Handle.Subsystem == this );

		UnregisterListenerInternal( Handle.Channel, Handle.ID );
	}
	else
	{
		UE_LOG( LogGameplayMessageSubsystem, Warning, TEXT( "Trying to unregister an invalid Handle." ) );
	}
}

void UARGameplayMessageSubsystem::K2_BroadcastMessage( FGameplayTag Channel, const int32& Message )
{
	checkNoEntry();
}

DEFINE_FUNCTION( UARGameplayMessageSubsystem::execK2_BroadcastMessage )
{
	P_GET_STRUCT( FGameplayTag, Channel );

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>( nullptr );
	void* MessagePtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>( Stack.MostRecentProperty );

	P_FINISH;

	if( ensure( ( StructProp != nullptr ) && ( StructProp->Struct != nullptr ) && ( MessagePtr != nullptr ) ) )
	{
		P_THIS->BroadcastMessageInternal( Channel, StructProp->Struct, MessagePtr );
	}

}



FGameplayMessageListenerHandle UARGameplayMessageSubsystem::RegisterListenerInternal( FGameplayTag Channel, TFunction<void( FGameplayTag, const UScriptStruct*, const void* )>&& Callback, const UScriptStruct* StructType )
{
	FChannelListenerList& List = ListenerMap.FindOrAdd( Channel );

	FGameplayMessageListenerData& Entry = List.Listeners.AddDefaulted_GetRef();
	Entry.ReceivedCallback = MoveTemp( Callback );
	Entry.ListenerStructType = StructType;
	Entry.bHadValidType = StructType != nullptr;
	Entry.HandleID = ++List.HandleID;

	return FGameplayMessageListenerHandle( this, Channel, Entry.HandleID );
}

void UARGameplayMessageSubsystem::BroadcastMessageInternal( FGameplayTag Channel, const UScriptStruct* StructType, const void* MessageBytes )
{

	if( UE::GameplayMessageSubsystem::ShouldLogMessages != 0 )
	{
		FString* pContextString = nullptr;
#if WITH_EDITOR
		if( GIsEditor )
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			pContextString = &GPlayInEditorContextString;
		}
#endif

		FString HumanReadableMessage;
		StructType->ExportText(/*out*/ HumanReadableMessage, MessageBytes, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr );
		UE_LOG( LogGameplayMessageSubsystem, Log, TEXT( "BroadcastMessage(%s, %s, %s)" ), pContextString ? **pContextString : *GetPathNameSafe( this ), *Channel.ToString(), *HumanReadableMessage );
	}

	if( const FChannelListenerList* ListenerList = ListenerMap.Find( Channel ) )
	{
		TArray<FGameplayMessageListenerData> ListenerArray( ListenerList->Listeners );

		for( const FGameplayMessageListenerData& Listener : ListenerArray )
		{
			if( Listener.bHadValidType && !Listener.ListenerStructType.IsValid() )
			{
				UE_LOG( LogGameplayMessageSubsystem, Warning, TEXT( "Listener struct type has gone invalid on Channel %s. Removing listener from list" ), *Channel.ToString() );
				UnregisterListenerInternal( Channel, Listener.HandleID );
				continue;
			}

			if( !Listener.bHadValidType || StructType->IsChildOf( Listener.ListenerStructType.Get() ) )
			{
				Listener.ReceivedCallback( Channel, StructType, MessageBytes );
			}
			else
			{
				UE_LOG( LogGameplayMessageSubsystem, Error, TEXT( "Struct type mismatch on channel %s (broadcast type %s, listener at %s was expecting type %s)" ),
						*Channel.ToString(),
						*StructType->GetPathName(),
						*Channel.ToString(),
						*Listener.ListenerStructType->GetPathName() );
			}
		}
	}
}

void UARGameplayMessageSubsystem::UnregisterListenerInternal( FGameplayTag Channel, int32 HandleID )
{
	if( FChannelListenerList* ListenerList = ListenerMap.Find( Channel ) )
	{
		int32 MatchIndex = ListenerList->Listeners.IndexOfByPredicate( [ID = HandleID]( const FGameplayMessageListenerData& Other ) { return Other.HandleID == ID; } );
		if( MatchIndex != INDEX_NONE )
		{
			ListenerList->Listeners.RemoveAtSwap( MatchIndex );
		}

		if( ListenerList->Listeners.Num() == 0 )
		{
			ListenerMap.Remove( Channel );
		}
	}
}
