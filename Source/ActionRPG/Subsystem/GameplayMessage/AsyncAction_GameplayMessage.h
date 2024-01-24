// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Delegates/Delegate.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "HAL/Platform.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Subsystem/GameplayMessage/ARGameplayMessageSubsystem.h"

#include "AsyncAction_GameplayMessage.generated.h"

class UScriptStruct;
class UWorld;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FAsyncGameplayMessageDelegate, UAsyncAction_GameplayMessage*, ProxyObject, FGameplayTag, ActualChannel );
/**
 *
 */

UCLASS( BlueprintType, meta = ( HasDedicatedAsyncNode ) )
class ACTIONRPG_API UAsyncAction_GameplayMessage : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:

	UFUNCTION( BlueprintCallable, Category = Messaging, meta = ( WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true" ) )
		static UAsyncAction_GameplayMessage* ListenForGameplayMessages( UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType );


	UFUNCTION( BlueprintCallable, CustomThunk, Category = "Messaging", meta = ( CustomStructureParam = "OutPayload" ) )
		bool GetPayload( UPARAM( ref ) int32& OutPayload );

	DECLARE_FUNCTION( execGetPayload );

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

private:
	void HandleMessageReceived( FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload );


public:
	UPROPERTY( BlueprintAssignable )
		FAsyncGameplayMessageDelegate OnMessageReceived;

private:

	const void* ReceivedMessagePayloadPtr = nullptr;

	TWeakObjectPtr<UWorld> WorldPtr;
	FGameplayTag ChannelToRegister;
	TWeakObjectPtr<UScriptStruct> MessageStructType = nullptr;

	FGameplayMessageListenerHandle ListenerHandle;

};
