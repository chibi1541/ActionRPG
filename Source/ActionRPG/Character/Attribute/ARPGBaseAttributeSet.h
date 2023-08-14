// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ARPGBaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 *
 */
UCLASS()
class ACTIONRPG_API UARPGBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UARPGBaseAttributeSet( const FObjectInitializer& ObjectInitializer );



public:

	UPROPERTY( BlueprintReadOnly, Category = "Status|STR" )
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS( UARPGBaseAttributeSet, Strength )

	UPROPERTY( BlueprintReadOnly, Category = "Status|AGI" )
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS( UARPGBaseAttributeSet, Agility )

	UPROPERTY( BlueprintReadOnly, Category = "Status|INT" )
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS( UARPGBaseAttributeSet, Intelligence )

	UPROPERTY( BlueprintReadOnly, Category = "Status|VIT" )
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS( UARPGBaseAttributeSet, Vitality )

	UPROPERTY( BlueprintReadOnly, Category = "Level" )
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS( UARPGBaseAttributeSet, CharacterLevel )

	//UPROPERTY( BlueprintReadOnly, Category = "MoveSpeed" )
	//FGameplayAttributeData MoveSpeed;
	//ATTRIBUTE_ACCESSORS( UARPGAttributeSetBase, MoveSpeed )

	//UPROPERTY( BlueprintReadOnly, Category = "AttackDamage" )
	//FGameplayAttributeData AttackDamage;
	//ATTRIBUTE_ACCESSORS( UARPGAttributeSetBase, AttackDamage )


protected:


};
