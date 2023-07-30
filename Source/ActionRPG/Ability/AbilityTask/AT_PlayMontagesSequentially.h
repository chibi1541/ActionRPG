// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"

#include "AT_PlayMontagesSequentially.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_DELEGATE_RetVal( bool, FPlayNextMontageCheckDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FMontageWaitSimpleDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMontagePlayDelegate, int, CurrentIndex );

/**
 *
 */
UCLASS()
class ACTIONRPG_API UAT_PlayMontagesSequentially : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	static UAT_PlayMontagesSequentially* CreatePlayMontagesSequentiallyProxy( UGameplayAbility* OwningAbility,
		FName TaskInstanceName, TArray<UAnimMontage*> MontageList, float Rate = 1.f, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f );

private:
	UFUNCTION( BlueprintCallable, Category = "Ability|Tasks", meta = ( DisplayName = "PlayMontagesSequentially",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE" ) )
	static UAT_PlayMontagesSequentially* K2_CreatePlayMontagesSequentiallyProxy( UGameplayAbility* OwningAbility,
		FName TaskInstanceName, TArray<UAnimMontage*> MontageList, float Rate = 1.f, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f );

protected:
	UFUNCTION()
	virtual void OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted );

	UFUNCTION()
	virtual void OnMontageInterrupted();

	UFUNCTION()
	virtual void OnMontageEnded( UAnimMontage* Montage, bool bInterrupted );

	virtual void Activate() override;

	virtual bool PlayNextMontage();

	virtual bool CanPlayNextMontage();

	virtual void ExternalCancel() override;

	virtual void OnDestroy( bool AbilityEnded ) override;

	virtual bool StopPlayingMontage();

	virtual bool IsContainMontage( UAnimMontage* AnimMontage );

public:

	const int GetMontageIndex() const { return MontagePlayIndex; }

	// Can Use Only C++
	FPlayNextMontageCheckDelegate	OnNextMontagePlayCheck;
	FMontagePlayDelegate OnPlayMontage;

	UPROPERTY( BlueprintAssignable )
	FMontageWaitSimpleDelegate	OnCompleted;

	UPROPERTY( BlueprintAssignable )
	FMontageWaitSimpleDelegate	OnBlendOut;

	UPROPERTY( BlueprintAssignable )
	FMontageWaitSimpleDelegate	OnInterrupted;

	UPROPERTY( BlueprintAssignable )
	FMontageWaitSimpleDelegate	OnCancelled;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> PlayMontageList;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	int MontagePlayIndex;
};
