// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"

#include "BaseCharacter.generated.h"

class UARPGAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayAbility;
class UAbilitySet;
class UARPGBaseAttributeSet;
class UGameplayEffect;

UCLASS( config = Game )
class ACTIONRPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	UARPGAbilitySystemComponent* GetARPGAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags( FGameplayTagContainer& TagContainer ) const override;

	virtual void BeginPlay();

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseCharacter|Attributes" )
	int32 GetCharacterLevel() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseCharacter|Attributes" )
	int32 GetVitality() const;

protected:
	virtual void InitAbilitySystem();

	virtual void InitializerAttributes();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Ability" )
	TObjectPtr<UARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability" )
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute" )
	TSubclassOf<UGameplayEffect> AttributeInitializer;

	TWeakObjectPtr<UARPGBaseAttributeSet> BaseAttributeSet;

};
