// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "ARCharacterStateComponent.generated.h"

class UGetHitComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UARCharacterStateComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARCharacterStateComponent();

	UARAbilitySystemComponent* GetARAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	void OnGetStuned( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
	void OnStunStateRemoved( const FActiveGameplayEffect& ActiveEffect );

public:
	void SetStiffEffectSpec( FGameplayEffectSpecHandle& SpecHandle );

	UFUNCTION(BlueprintCallable, Category = "CharacterState|Stun")
	const bool GetStunState() const;

private:
	TWeakObjectPtr<UGetHitComponent> GetHitComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "ASC", meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UARAbilitySystemComponent> AbilitySystemComponent;

	// State Variable
	bool IsStunned;

public:

	UPROPERTY( BlueprintReadWrite, Category = "Stiff" )
	FGameplayEffectSpecHandle StiffEffectSpecHandle;
};
