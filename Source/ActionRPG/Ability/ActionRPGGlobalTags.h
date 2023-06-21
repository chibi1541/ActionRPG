// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"


struct ACTIONRPG_API FActionRPGGlobalTags : public FGameplayTagNativeAdder
{
	// Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;

	FORCEINLINE static const FActionRPGGlobalTags& Get() { return ActionRPGTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		InputTag_Move = Manager.AddNativeGameplayTag(TEXT("InputTag.Move"));
		InputTag_Look = Manager.AddNativeGameplayTag(TEXT("InputTag.Look"));
	}

private:
	static FActionRPGGlobalTags ActionRPGTags;

};
