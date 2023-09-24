// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ARIntRefAttribSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARIntRefAttribSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()

public:

	virtual void PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue ) override;
	virtual void PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data ) override;

public:

	UPROPERTY( BlueprintReadOnly, Category = "Mana" )
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS( UARIntRefAttribSet, MaxMana )

	UPROPERTY( BlueprintReadOnly, Category = "Mana" )
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS( UARIntRefAttribSet, Mana )

	UPROPERTY( BlueprintReadOnly, Category = "Mana" )
	FGameplayAttributeData ManaRegene;
	ATTRIBUTE_ACCESSORS( UARIntRefAttribSet, ManaRegene )
};
