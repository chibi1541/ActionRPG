// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AT_PlayMontageAndWait.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FMontageWaitDelegate );

/**
 *
 */
UCLASS()
class ACTIONRPG_API UAT_PlayMontageAndWait : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	FMontageWaitDelegate OnCompleted;

	UPROPERTY()
	FMontageWaitDelegate OnBlendOut;

	UPROPERTY()
	FMontageWaitDelegate OnInterrupted;

	UPROPERTY()
	FMontageWaitDelegate OnCancelled;

	UFUNCTION()
	void OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted );

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageEnded( UAnimMontage* Montage, bool bInterrupted );

	static UAT_PlayMontageAndWait* CreatePlayMontageAndWait(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		float Rate = 1.f,
		FName StartSeciton = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.f,
		float StartTimeSeconds = 0.f );

protected:
	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual void OnDestroy( bool bInOwnerFinished ) override;

	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	FName StartSection;

	float Rate;

	float AnimRootMotionTranslationScale;

	float StartTimeSeconds;

	bool bStopWhenAbilityEnds;

	
};
