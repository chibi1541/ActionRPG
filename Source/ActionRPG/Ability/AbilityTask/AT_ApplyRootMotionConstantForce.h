// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotion_Base.h"

#include "AT_ApplyRootMotionConstantForce.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FApplyRootMotionConstantForceDelegate );

class UCurveFloat;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UAT_ApplyRootMotionConstantForce : public UAbilityTask_ApplyRootMotion_Base
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	FApplyRootMotionConstantForceDelegate OnFinish;

	static UAT_ApplyRootMotionConstantForce* CreateApplyRootMotionConstantForce
	(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		FVector WorldDirection,
		float Strength,
		float Duration,
		bool bIsAdditive,
		UCurveFloat* StrengthOverTime,
		ERootMotionFinishVelocityMode VelocityOnFinishMode,
		FVector SetVelocityOnFinish,
		float ClampVelocityOnFinish,
		bool bEnableGravity
	);

private:
	virtual void OnDestroy( bool bInOwnerFinished ) override;
	virtual void TickTask(float DeltaTime) override;

protected:
	void SharedInitAndApply() override;

	FVector WorldDirection;

	float Strength;

	float Duration;

	bool bIsAdditive;

	UPROPERTY( EditAnywhere, Category = "StrengthCurve", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UCurveFloat> StrengthOverTime;

	bool bEnableGravity;

};
