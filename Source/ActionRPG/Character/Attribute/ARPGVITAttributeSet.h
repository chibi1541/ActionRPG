// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ARPGVITAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARPGVITAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UARPGVITAttributeSet( const FObjectInitializer& ObjectInitializer );


	virtual void PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue ) override;
	virtual void PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data ) override;

protected:
	void AdjustAttributeForMaxChange( FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty );

public:

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, MaxHealth )

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, Health )

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData HealthRegene;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, HealthRegene )

	UPROPERTY( BlueprintReadOnly, Category = "Stamina" )
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, MaxStamina )

	UPROPERTY( BlueprintReadOnly, Category = "Stamina" )
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, Stamina )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, Defence )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData MaxShieldGauge;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, MaxShieldGauge )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData ShieldGauge;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, ShieldGauge )

	UPROPERTY( BlueprintReadOnly, Category = "Characteristic" )
	FGameplayAttributeData HealthRate;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, HealthRate )

	UPROPERTY( BlueprintReadOnly, Category = "Characteristic" )
	FGameplayAttributeData StaminaRate;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, StaminaRate )

	UPROPERTY( BlueprintReadOnly, Category = "Characteristic" )
	FGameplayAttributeData DefenceRate;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, DefenceRate )

	UPROPERTY( BlueprintReadOnly, Category = "Characteristic" )
	FGameplayAttributeData ShieldGaugeRate;
	ATTRIBUTE_ACCESSORS( UARPGVITAttributeSet, ShieldGaugeRate )
};
