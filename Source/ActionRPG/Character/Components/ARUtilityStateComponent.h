// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"

#include "ARUtilityStateComponent.generated.h"


class UBlackboardComponent;
class UAbilitySystemComponent;
class UARVitRefAttribSet;
class UARIntRefAttribSet;


UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UARUtilityStateComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:

	virtual void BeginPlay() override;

	void InitializeOnPossessed( UBlackboardComponent* BlackboardComponent );

	void UnPossessController();

protected:
	void OnHealthChange( const FOnAttributeChangeData& Data );

	void OnStaminaChange( const FOnAttributeChangeData& Data );

	void OnManaChange( const FOnAttributeChangeData& Data );

	void OnShieldGaugeChange( const FOnAttributeChangeData& Data );

private:
	void BindAttributeDelegate();

public:
	static const FName HealthRateKey;
	static const FName StaminaRateKey;
	static const FName ManaRateKey;
	static const FName ShieldGaugeRateKey;

protected:

	TWeakObjectPtr<UBlackboardComponent> Blackboard;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<const UARVitRefAttribSet> HealthAttrib;

	TWeakObjectPtr<const UARIntRefAttribSet> ManaAttrib;

	FDelegateHandle HealthRateHandle;
	FDelegateHandle StaminaRateHandle;
	FDelegateHandle ManaRateHandle;
	FDelegateHandle ShieldRateHandle;

};
