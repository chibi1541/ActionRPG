// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ARAgiRefAttribSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARAgiRefAttribSet : public UAttributeSet
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY( BlueprintReadOnly, Category = "MoveSpeed" )
	FGameplayAttributeData ModifiedMoveSpeed;
	ATTRIBUTE_ACCESSORS( UARAgiRefAttribSet, ModifiedMoveSpeed )

	UPROPERTY( BlueprintReadOnly, Category = "AttackSpeed" )
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS( UARAgiRefAttribSet, AttackSpeed )
};
