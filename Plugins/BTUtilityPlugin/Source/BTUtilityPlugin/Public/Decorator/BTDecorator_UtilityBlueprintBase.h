// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Decorator/BTDecorator_UtilityFunction.h"

#include "BTDecorator_UtilityBlueprintBase.generated.h"

/**
 * 
 */
UCLASS( Abstract, Blueprintable )
class BTUTILITYPLUGIN_API UBTDecorator_UtilityBlueprintBase : public UBTDecorator_UtilityFunction
{

	GENERATED_UCLASS_BODY()

protected:
	virtual float CalculateUtilityValue( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) const override;


	virtual void SetOwner( AActor* ActorOwner ) override;

#if WITH_EDITOR
	virtual bool UsesBlueprint() const override;

#endif // WITH_EDITOR

protected:
	UFUNCTION( BlueprintImplementableEvent, Category = Utility )
		float CalculateUtility( AAIController* OwnerController, APawn* ControlledPawn ) const;

protected:
	UPROPERTY( Transient )
		AAIController* AIOwner;

	UPROPERTY( Transient )
		AActor* ActorOwner;

};
