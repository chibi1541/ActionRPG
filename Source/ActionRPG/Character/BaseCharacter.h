// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Character/ARCharacterEnumDef.h"

#include "BaseCharacter.generated.h"

class UARAbilitySystemComponent;
class UAbilitySystemComponent;
class UGameplayAbility;
class UAbilitySet;
class UARBaseAttribSet;
class UGameplayEffect;
class UARCharacterStateComponent;
class UARTargetingComponent;
class UGameplayEffectSet;
class UARUtilityStateComponent;

UCLASS( config = Game )
class ACTIONRPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter( const FObjectInitializer& ObjectInitializer );

	UARAbilitySystemComponent* GetARAbilitySystemComponent() const { return AbilitySystemComp; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags( FGameplayTagContainer& TagContainer ) const override;

	virtual void BeginPlay();

	UFUNCTION( BlueprintCallable, Category = "ARCharacter|StateComponent" )
		UARCharacterStateComponent* GetCharacterStateComponenet() const;

	UFUNCTION( BlueprintCallable, Category = "ARCharacter|TargetingComponent" )
		UARTargetingComponent* GetTargetingComponent() const;

	UFUNCTION( BlueprintCallable, Category = "ARCharacter|Component" )
		USceneComponent* GetConditionEffectPosComp() const;

	UFUNCTION( BlueprintCallable )
		FVector GetCurrentVelocity() const;

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

	UFUNCTION( BlueprintCallable )
		virtual void FinishDying();

	virtual void Die();

	virtual bool GetDeadState() const;

protected:
	virtual void InitAbilitySystem();

	virtual void InitializeAttributes();

	virtual void InitPassiveEffect();

	virtual void OnSpeedChange( const FOnAttributeChangeData& Data );

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components|State" )
		TObjectPtr<UARCharacterStateComponent> CharacterStateComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Components|State" )
		TObjectPtr<UARUtilityStateComponent> UtilityStateComponent;

	TObjectPtr<UARAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		TObjectPtr<UARTargetingComponent> TargetingComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		TObjectPtr<USceneComponent> ConditionEffectPosComp;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability" )
		TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Ability" )
		TArray<TObjectPtr<UGameplayEffectSet>> GameplayEffectSets;

	UPROPERTY()
		TObjectPtr<UARBaseAttribSet> BaseAttribSet;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "CharacterType" )
		ECharacterType CharacterType;

public:
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
};
