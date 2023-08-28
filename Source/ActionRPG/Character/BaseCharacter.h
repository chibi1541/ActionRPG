// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Character/ARCharacterEnumDef.h"

#include "BaseCharacter.generated.h"

class UARPGAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayAbility;
class UAbilitySet;
class UARBaseAttribSet;
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

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseAttributes" )
		int32 GetCharacterLevel() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseAttributes" )
		int32 GetStrength() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseAttributes" )
		int32 GetAgility() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseAttributes" )
		int32 GetIntelligence() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|BaseAttributes" )
		int32 GetVitality() const;


protected:
	virtual void InitAbilitySystem();

	virtual void InitializerAttributes();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Ability" )
		TObjectPtr<UARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability" )
		TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Base" )
		TSubclassOf<UGameplayEffect> BaseAttribInitializer;

	UPROPERTY()
		TObjectPtr<UARBaseAttribSet> BaseAttribSet;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CharacterType" )
		ECharacterType CharacterType;

public:
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
};
