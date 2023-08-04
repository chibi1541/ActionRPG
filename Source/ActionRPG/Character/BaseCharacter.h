// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "BaseCharacter.generated.h"

class UARPGAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayAbility;
class UAbilitySet;

UCLASS( config = Game )
class ACTIONRPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	virtual void BeginPlay();

protected:

	virtual void InitAbilitySystem();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = ( AllowPrivateAccess = "true" ) )
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

};
