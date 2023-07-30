// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityTask/AT_PlayMontagesSequentially.h"

#include "AT_PlayMontagesWithGameplayEvent.generated.h"

/**
 * 
 */
class UAnimMontage;
struct FGameplayTag;
struct FGameplayEventData;

UCLASS()
class ACTIONRPG_API UAT_PlayMontagesWithGameplayEvent : public UAT_PlayMontagesSequentially
{
	GENERATED_UCLASS_BODY()



public:
	static UAT_PlayMontagesWithGameplayEvent* CreatePlayMontagesWithGameplayEventProxy( UGameplayAbility* OwningAbility,
		FName TaskInstanceName, TArray<UAnimMontage*> MontageList, FGameplayTag EventTag, bool OnlyTriggerOnce = false, float AnimRate = 1.f, bool StopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f );

private:
	UFUNCTION( BlueprintCallable, Category = "Ability|Tasks", meta = ( DisplayName = "PlayMontagesWithGameplayEvent",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE" ) )
		static UAT_PlayMontagesWithGameplayEvent* K2_CreatePlayMontagesWithGameplayEventProxy( UGameplayAbility* OwningAbility,
		FName TaskInstanceName, TArray<UAnimMontage*> MontageList, FGameplayTag EventTag, bool OnlyTriggerOnce = false, float AnimRate = 1.f, bool StopWhenAbilityEnds = true, float fAnimRootMotionTranslationScale = 1.f );
	
protected:
	virtual void OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted ) override;

	virtual void Activate() override;

	virtual void GameplayEventCallback( const FGameplayEventData* Payload );

	virtual void OnDestroy( bool AbilityEnded ) override;

private:
	FGameplayTag CheckEventTag;

	FDelegateHandle CheckEventHandle;

	UPROPERTY()
	bool bOnlyTriggerOnce;

};
