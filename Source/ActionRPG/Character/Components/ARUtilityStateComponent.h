// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"

#include "ARUtilityStateComponent.generated.h"


class UBlackboardComponent;
class UARAbilitySystemComponent;
class UARVitRefAttribSet;
class UARIntRefAttribSet;


UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UARUtilityStateComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void InitializeOnPossessed( UBlackboardComponent* BlackboardComponent );

protected:
	virtual void OnHealthChange( const FOnAttributeChangeData& Data );


public:
	static const FName HealthRateKey;
	static const FName StaminaRateKey;
	static const FName ManaRateKey;
	static const FName ShieldGaugeRateKey;

protected:
	UPROPERTY( BlueprintReadOnly )
		TObjectPtr<UBlackboardComponent> Blackboard;


	TWeakObjectPtr<const UARVitRefAttribSet> HealthAttrib;

	TWeakObjectPtr<const UARIntRefAttribSet> ManaAttrib;

};
