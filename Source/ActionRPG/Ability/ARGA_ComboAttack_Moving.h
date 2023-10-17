// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/ARGA_ComboAttack_Melee.h"
#include "ARGA_ComboAttack_Moving.generated.h"

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARGA_ComboAttack_Moving : public UARGA_ComboAttack_Melee
{
	GENERATED_UCLASS_BODY()


protected:
	virtual void OnPlayMontage( int CurrentIndex ) override;
	
	UFUNCTION()
	void CallLambda() { LambdaFunc(); }

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Distance" )
		TArray<float> MoveDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Distance" )
		TArray<float> MoveDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Effect" )
		TSubclassOf<UGameplayEffect> KnockbackEffect;

	TFunction<void()> LambdaFunc;
	

};
