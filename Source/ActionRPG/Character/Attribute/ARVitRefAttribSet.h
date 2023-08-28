// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

#include "ARVitRefAttribSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARVitRefAttribSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UARVitRefAttribSet( const FObjectInitializer& ObjectInitializer );


	virtual void PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue ) override;
	virtual void PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data ) override;

protected:
	void AdjustAttributeForMaxChange( FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty );

public:

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, MaxHealth )

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, Health )

	UPROPERTY( BlueprintReadOnly, Category = "HP" )
	FGameplayAttributeData HealthRegene;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, HealthRegene )

	UPROPERTY( BlueprintReadOnly, Category = "Stamina" )
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, MaxStamina )

	UPROPERTY( BlueprintReadOnly, Category = "Stamina" )
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, Stamina )

	UPROPERTY( BlueprintReadOnly, Category = "Stamina" )
	FGameplayAttributeData StaminaRegene;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, StaminaRegene )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, Defence )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData MaxShieldGauge;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, MaxShieldGauge )

	UPROPERTY( BlueprintReadOnly, Category = "Defence" )
	FGameplayAttributeData ShieldGauge;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, ShieldGauge )

	UPROPERTY( BlueprintReadOnly, Category = "Meta_Attribute" )
	FGameplayAttributeData ReceivedDamage;
	ATTRIBUTE_ACCESSORS( UARVitRefAttribSet, ReceivedDamage )

};
