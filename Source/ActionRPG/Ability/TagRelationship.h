// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UObject/UObjectGlobals.h"

#include "TagRelationship.generated.h"

USTRUCT()
struct FAbilityTagRelationship
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action" ))
	FGameplayTag AbilityTag;

	// ���� AbilityTag�� ���� ���ܵǴ� Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	// ���� AbilityTag�� ���� ĵ���Ǵ� Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	// ���� AbilityTag�� Ȱ��ȭ �Ǳ� ���� �ʿ��� Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	// ���� AbilityTag�� Ȱ��ȭ �Ǵ� ���� ���� Tags
	UPROPERTY( EditAnywhere, Category = Ability )
	FGameplayTagContainer ActivationBlockedTags;

};

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UTagRelationship : public UDataAsset
{
	GENERATED_BODY()
	
};
