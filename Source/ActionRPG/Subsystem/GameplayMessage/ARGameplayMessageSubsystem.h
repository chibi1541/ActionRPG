// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Delegates/IDelegateInstance.h"
#include "GameplayTagContainer.h"
#include "HAL/Platform.h"
#include "Logging/LogMacros.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/Function.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ARGameplayMessageSubsystem.generated.h"

/**
 *
 */

DECLARE_LOG_CATEGORY_EXTERN( LogGameplayMessageSubsystem, Log, All );

class UAsyncAction_GameplayMessage;


USTRUCT( BlueprintType )
struct ACTIONRPG_API FGameplayMessageListenerHandle
{
public:
	GENERATED_BODY()

		FGameplayMessageListenerHandle() {}

	void Unregister();

	bool IsValid() const { return ID != 0; }

private:
	UPROPERTY( Transient )
		TWeakObjectPtr<UARGameplayMessageSubsystem> Subsystem;

	UPROPERTY( Transient )
		FGameplayTag Channel;

	UPROPERTY( Transient )
		int32 ID = 0;

	FDelegateHandle StateClearedHandle;

	friend UARGameplayMessageSubsystem;

	FGameplayMessageListenerHandle( UARGameplayMessageSubsystem* InSubsystem, FGameplayTag InChannel, int32 InID ) : Subsystem( InSubsystem ), Channel( InChannel ), ID( InID ) {}
};

USTRUCT()
struct FGameplayMessageListenerData
{
	GENERATED_BODY()

		TFunction<void( FGameplayTag, const UScriptStruct*, const void* )> ReceivedCallback;

	int32 HandleID;

	TWeakObjectPtr<const UScriptStruct> ListenerStructType = nullptr;
	bool bHadValidType = false;
};

UCLASS()
class ACTIONRPG_API UARGameplayMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

		friend UAsyncAction_GameplayMessage;

public:


	static UARGameplayMessageSubsystem& Get( const UObject* WorldContextObject );

	static bool HasInstance( const UObject* WorldContextObject );

	virtual void Deinitialize() override;

public:
	template <typename FMessageStructType>
	FGameplayMessageListenerHandle RegisterListener( FGameplayTag Channel, TFunction<void( FGameplayTag, const FMessageStructType& )>&& Callback )
	{
		auto ThunkCallback = [InnerCallback = MoveTemp( Callback )]( FGameplayTag ActualTag, const UScriptStruct* SenderStructType, const void* SenderPayload )
		{
			InnerCallback( ActualTag, *reinterpret_cast< const FMessageStructType* >( SenderPayload ) );
		};

		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		return RegisterListenerInternal( Channel, ThunkCallback, StructType, MatchType );
	}

	template <typename FMessageStructType, typename TOwner = UObject>
	FGameplayMessageListenerHandle RegisterListener( FGameplayTag Channel, TOwner* Object, void( TOwner::* Function )( FGameplayTag, const FMessageStructType& ) )
	{
		TWeakObjectPtr<TOwner> WeakObject( Object );
		return RegisterListener<FMessageStructType>(
			Channel,
			[WeakObject, Function]( FGameplayTag Channel, const FMessageStructType& Payload )
			{
				if( TOwner* StrongObject = WeakObject.Get() )
				{
					( StrongObject->*Function )( Channel, Payload );
				}
			} );
	}

	template <typename FMessageStructType>
	void BroadcastMessage( FGameplayTag Channel, const FMessageStructType& Message )
	{
		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		BroadcastMessageInternal( Channel, StructType, &Message );
	}

	void UnregisterListener( FGameplayMessageListenerHandle Handle );

protected:
	
	UFUNCTION( BlueprintCallable, CustomThunk, Category = Messaging, meta = ( CustomStructureParam = "Message", AllowAbstract = "false", DisplayName = "Broadcast Message" ) )
		void K2_BroadcastMessage( FGameplayTag Channel, const int32& Message );

	DECLARE_FUNCTION( execK2_BroadcastMessage );


private:

	FGameplayMessageListenerHandle RegisterListenerInternal(
		FGameplayTag Channel,
		TFunction<void( FGameplayTag, const UScriptStruct*, const void* )>&& Callback,
		const UScriptStruct* StructType );

	void BroadcastMessageInternal( FGameplayTag Channel, const UScriptStruct* StructType, const void* MessageBytes );

	void UnregisterListenerInternal( FGameplayTag Channel, int32 HandleID );


private:

	struct FChannelListenerList
	{
		TArray<FGameplayMessageListenerData> Listeners;
		int32 HandleID = 0;
	};

	TMap<FGameplayTag, FChannelListenerList> ListenerMap;


};
