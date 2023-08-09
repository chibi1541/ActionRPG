// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AIController.h"

#include "BaseAIController.generated.h"

/**
 *
 */
class UBehaviorTree;
class UBlackboardData;
class UARPGAbilitySystemComponent;

UCLASS()
class ACTIONRPG_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

	ABaseAIController( const FObjectInitializer& ObjectInitializer );

public:
	virtual void OnPossess( APawn* InPawn ) override;

	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName ASCKey;
	
private:
	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const;

protected:
	UPROPERTY( VisibleAnywhere, Category = "AILogic", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY( VisibleAnywhere, Category = "AILogic", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UBlackboardData> BlackboardData;


};
