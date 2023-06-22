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

	// 현재 AbilityTag에 의해 차단되는 Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	// 현재 AbilityTag에 의해 캔슬되는 Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	// 현재 AbilityTag가 활성화 되기 위해 필요한 Tags
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	// 현재 AbilityTag가 활성화 되는 것을 막는 Tags
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
