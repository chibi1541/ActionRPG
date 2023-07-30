// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"

#include "AT_WaitInputRelease.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FReleaseDelegate, float, TimeHeld );
/**
 *
 */
UCLASS()
class ACTIONRPG_API UAT_WaitInputRelease : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = ( HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE" ) )
	static UAT_WaitInputRelease* CreateWaitInputRelease( UGameplayAbility* OwningAbility );

private:
	UFUNCTION()
	void OnReleaseCallBack();
	
	virtual void Activate() override;

protected:
	UPROPERTY( BlueprintAssignable )
	FReleaseDelegate OnRelease;

	float StartTime;
	FDelegateHandle DelegateHandle;

};
