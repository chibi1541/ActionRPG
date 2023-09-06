// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/ObjectMacros.h"

#include "ARGameplayAbility.generated.h"

UENUM( BlueprintType )
enum class EARAbilityActivationPolicy : uint8
{
	OnInputTriggered,

	WhileInputHeld
};


class UAbilitySystemComponent;
struct FGameplayTagContainer;

DECLARE_MULTICAST_DELEGATE( FInputProcessDelegate );

UCLASS()
class ACTIONRPG_API UARGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UARGameplayAbility(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool DoesAbilitySatisfyTagRequirements( const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags ) const override;

	UFUNCTION( BlueprintImplementableEvent, Category = Ability, DisplayName = "InputPressed", meta = ( ScriptName = "InputPressed" ) )
	bool K2_InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo );

	bool bInputPressBlueprintCanUse;

	UFUNCTION( BlueprintImplementableEvent, Category = Ability, DisplayName = "InputReleased", meta = ( ScriptName = "InputReleased" ) )
	bool K2_InputReleased( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo );

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	EARAbilityActivationPolicy GetActivationPolicyType() const { return ActionPolicyType; }

	virtual void InputReleased( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) override;

	virtual void InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo ) override;

protected:
	UPROPERTY( EditAnywhere, Category = "Ability|Activation" )
	EARAbilityActivationPolicy ActionPolicyType;

	bool bInputReleaseBlueprintCanUse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Activation" )
	bool bActivateAbilityOnGranted;

public:
	FInputProcessDelegate OnReleased;

	FInputProcessDelegate OnPressed;
};
