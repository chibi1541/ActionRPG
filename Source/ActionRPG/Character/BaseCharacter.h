// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UAbilitySet;

UCLASS( config = Game )
class ACTIONRPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	virtual void BeginPlay();

protected:
	// To be Deleted
	virtual void AddStartupGameplayAbilities();

	virtual void InitAbilitySet();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = ( AllowPrivateAccess = "true" ) )
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = ( AllowPrivateAccess = "true" ) )
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

};
