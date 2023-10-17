// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "UObject/Interface.h"

#include "ARUsedTraceHitInterface.generated.h"

class UHitTraceComponent;

UINTERFACE( MinimalAPI, BlueprintType, Blueprintable )
class UARUsedTraceHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
* The interface for GameplayAbilitise With HitTraceComponent
 *
 */
class ACTIONRPG_API IARUsedTraceHitInterface
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "HitTrace|Init" )
		void InitHitTraceOnHit( UHitTraceComponent* Component );

		virtual void InitHitTraceOnHit_Implementation( UHitTraceComponent* Component );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "HitTrace|Init" )
		void InitDelegateOnHit();

	virtual void InitDelegateOnHit_Implementation();

	UFUNCTION( BlueprintNativeEvent, Category = "HitTrace|OnHit" )
		void OnHit( FHitResult HitResult );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent, Category = "HitTrace|OnHit" )
		void ResetHitTraceComp();

	virtual void ResetHitTraceComp_Implementation();

protected:
	TWeakObjectPtr<UHitTraceComponent> HitTraceComp;
};
