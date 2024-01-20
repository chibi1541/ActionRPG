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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FAttribChanged, float, OldValue, float, NewValue );


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
		void OnGuard( const FGameplayEffectSpec& EffectSpec );

	UFUNCTION()
		void OnGuardRemoved( const FActiveGameplayEffect& ActiveEffect );

	#pragma region Attrib Changed Function
	virtual void OnMaxHealthChange( const FOnAttributeChangeData& Data );

	virtual void OnHealthChange( const FOnAttributeChangeData& Data );

	virtual void OnMaxStaminaChange( const FOnAttributeChangeData& Data );

	virtual void OnStaminaChange( const FOnAttributeChangeData& Data );

	virtual void OnMaxManaChange( const FOnAttributeChangeData& Data );

	virtual void OnManaChange( const FOnAttributeChangeData& Data );

	virtual void OnMaxShieldGaugeChange( const FOnAttributeChangeData& Data );

	virtual void OnShieldGaugeChange( const FOnAttributeChangeData& Data );
	#pragma endregion


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

	#pragma region Attrib Changed Event

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnCurHealthChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnMaxHealthChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnCurManaChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnMaxManaChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnCurStaminaChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnMaxStaminaChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnCurShieldGaugeChanged;

	UPROPERTY( BlueprintAssignable )
		FAttribChanged OnMaxShieldGaugeChanged;

	#pragma endregion 
	

};
