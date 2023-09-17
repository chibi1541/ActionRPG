// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"


struct ACTIONRPG_API FActionRPGGlobalTags : public FGameplayTagNativeAdder
{
	// Input Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;

	// Ability Tags (Ability.Action)
	FGameplayTag AbilityActionTag_Guard;

	// State Tags (Ability.State)
	FGameplayTag AbilityStateTag_Guard;

	// Condition Tags
	FGameplayTag CharacterStateTag_Stiff;
	FGameplayTag CharacterStateTag_Stun;
	FGameplayTag CharacterStateTag_Provoked;
	FGameplayTag CharacterStateTag_Exhausted;
	FGameplayTag CharacterStateTag_SpendStamina;
	FGameplayTag CharacterStateTag_FullStamina;

	// Cost Tags
	FGameplayTag AbilityCostTag_Stamina;

	// Extra Damage Tag
	FGameplayTag ExtraDamageTag;

	FORCEINLINE static const FActionRPGGlobalTags& Get() { return ActionRPGTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		InputTag_Move = Manager.AddNativeGameplayTag(TEXT("InputTag.Move"));
		InputTag_Look = Manager.AddNativeGameplayTag(TEXT("InputTag.Look"));
		
		AbilityActionTag_Guard = Manager.AddNativeGameplayTag( TEXT( "Ability.Action.Guard" ) );
		AbilityStateTag_Guard = Manager.AddNativeGameplayTag( TEXT( "Ability.State.Guard" ) );
		
		CharacterStateTag_Stiff = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Stiff" ) );
		CharacterStateTag_Stun = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Stun" ) );
		CharacterStateTag_Provoked = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Provoked" ) );
		CharacterStateTag_Exhausted = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Exhausted" ) );
		CharacterStateTag_SpendStamina = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.SpendStamina" ) );
		CharacterStateTag_FullStamina = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.FullStamina" ) );
		
		AbilityCostTag_Stamina = Manager.AddNativeGameplayTag( TEXT( "Ability.Cost.Stamina" ) );
		ExtraDamageTag = Manager.AddNativeGameplayTag( TEXT( "Ability.Value.ExtraDamage" ) );
	}

private:
	static FActionRPGGlobalTags ActionRPGTags;

};
