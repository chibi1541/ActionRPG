// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "ARCharacterStateComponent.generated.h"

class UGetHitComponent;
class UARVitRefAttribSet;
class UARIntRefAttribSet;
class ABaseCharacter;

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

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason );

	UFUNCTION()
		void OnGetStuned( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
		void OnStunStateRemoved( const FActiveGameplayEffect& ActiveEffect );

	UFUNCTION()
		void OnProvoked( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
		void OnRefreshProvoked( FActiveGameplayEffect& EffectSpec );

	UFUNCTION()
		void OnGuard( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
		void OnGuardRemoved( const FActiveGameplayEffect& ActiveEffect );

	virtual void OnStaminaChange( const FOnAttributeChangeData& Data );

	virtual void OnHealthChange( const FOnAttributeChangeData& Data );

	virtual void OnManaChange( const FOnAttributeChangeData& Data );

	virtual void OnShieldGaugeChange( const FOnAttributeChangeData& Data );

public:
	UFUNCTION( BlueprintCallable, Category = "CharacterState|Stiff" )
		void SetStiffEffectSpec( const FGameplayEffectSpecHandle& SpecHandle );

	UFUNCTION( BlueprintCallable, Category = "CharacterState|Stun" )
		bool GetStunState() const;

	UFUNCTION( BlueprintCallable, Category = "CharacterState|Dead" )
		bool GetDeadState() const;

	UFUNCTION( BlueprintCallable, Category = "CharacterState|Guard" )
		bool GetGuardState() const;

private:
	TWeakObjectPtr<UGetHitComponent> GetHitComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "ASC", meta = ( AllowPrivateAccess = "true" ) )
		TObjectPtr<UARAbilitySystemComponent> AbilitySystemComponent;

	// State Variable
	bool IsStunned;

	bool IsDead;

	bool IsGuard;

	TWeakObjectPtr<const UARVitRefAttribSet> HealthAttrib;

	TWeakObjectPtr<const UARIntRefAttribSet> ManaAttrib;

	int TargetIndex;

public:

	UPROPERTY( BlueprintReadWrite, Category = "Stiff" )
		FGameplayEffectSpecHandle StiffEffectSpecHandle;

};
