// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ARBaseAttribSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 *
 */
UCLASS()
class ACTIONRPG_API UARBaseAttribSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UARBaseAttribSet( const FObjectInitializer& ObjectInitializer );

	virtual void PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue ) override;

public:

	UPROPERTY( BlueprintReadOnly, Category = "Status|Level" )
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, CharacterLevel )

	UPROPERTY( BlueprintReadOnly, Category = "Status|STR" )
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, Strength )

	UPROPERTY( BlueprintReadOnly, Category = "Status|AGI" )
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, Agility )

	UPROPERTY( BlueprintReadOnly, Category = "Status|INT" )
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, Intelligence )

	UPROPERTY( BlueprintReadOnly, Category = "Status|VIT" )
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, Vitality )

	UPROPERTY( BlueprintReadOnly, Category = "Status|MoveSpeed" )
	FGameplayAttributeData BaseMoveSpeed;
	ATTRIBUTE_ACCESSORS( UARBaseAttribSet, BaseMoveSpeed )
};
