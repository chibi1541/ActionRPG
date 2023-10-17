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
	FGameplayTag InputTag_Confirm;
	FGameplayTag InputTag_Attack;

	// Ability Tags (Ability.Action)
	FGameplayTag AbilityActionTag_Guard;

	// State Tags (Ability.State)
	FGameplayTag AbilityStateTag_Guard;
	FGameplayTag AbilityStateTag_TargetingCancel;
	FGameplayTag AbilityStateTag_Sprint;
	FGameplayTag AbilityStateTag_Attack;
	FGameplayTag AbilityStateTag_NextAttack;

	// Condition Tags
	FGameplayTag CharacterStateTag_Stiff;
	FGameplayTag CharacterStateTag_Stun;
	FGameplayTag CharacterStateTag_Provoked;
	FGameplayTag CharacterStateTag_Exhausted;
	FGameplayTag CharacterStateTag_SpendStamina;
	FGameplayTag CharacterStateTag_FullStamina;
	FGameplayTag CharacterStateTag_FullHealth;
	FGameplayTag CharacterStateTag_FullMana;

	// Cost Tags
	FGameplayTag AbilityCostTag_Stamina;

	// Extra Damage Tag
	FGameplayTag ExtraDamageTag;
	FGameplayTag ExtraDamageCoeffTag;

	// Cooldown Duration Tag
	FGameplayTag CooldownDurationTag;

	FORCEINLINE static const FActionRPGGlobalTags& Get() { return ActionRPGTags; }

protected:
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		InputTag_Move = Manager.AddNativeGameplayTag( TEXT( "InputTag.Move" ) );
		InputTag_Look = Manager.AddNativeGameplayTag( TEXT( "InputTag.Look" ) );
		InputTag_Confirm = Manager.AddNativeGameplayTag( TEXT( "InputTag.Confirm" ) );
		InputTag_Attack = Manager.AddNativeGameplayTag( TEXT( "InputTag.Attack" ) );

		AbilityActionTag_Guard = Manager.AddNativeGameplayTag( TEXT( "Ability.Action.Guard" ) );

		AbilityStateTag_Guard = Manager.AddNativeGameplayTag( TEXT( "Ability.State.Guard" ) );
		AbilityStateTag_TargetingCancel = Manager.AddNativeGameplayTag( TEXT( "Ability.State.TargetingCancel" ) );
		AbilityStateTag_Sprint = Manager.AddNativeGameplayTag( TEXT( "Ability.State.Sprint" ) );
		AbilityStateTag_Attack = Manager.AddNativeGameplayTag( TEXT( "Ability.State.Attack" ) );
		AbilityStateTag_NextAttack = Manager.AddNativeGameplayTag( TEXT( "Ability.State.NextAttack" ) );

		CharacterStateTag_Stiff = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Stiff" ) );
		CharacterStateTag_Stun = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Stun" ) );
		CharacterStateTag_Provoked = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Provoked" ) );
		CharacterStateTag_Exhausted = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.Exhausted" ) );
		CharacterStateTag_SpendStamina = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.SpendStamina" ) );
		CharacterStateTag_FullStamina = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.FullStamina" ) );
		CharacterStateTag_FullHealth = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.FullHealth" ) );
		CharacterStateTag_FullMana = Manager.AddNativeGameplayTag( TEXT( "Gameplay.Character.State.FullMana" ) );

		AbilityCostTag_Stamina = Manager.AddNativeGameplayTag( TEXT( "Ability.Cost.Stamina" ) );

		ExtraDamageTag = Manager.AddNativeGameplayTag( TEXT( "Ability.Value.ExtraDamage" ) );
		ExtraDamageTag = Manager.AddNativeGameplayTag( TEXT( "Ability.Value.ExtraDamageCoeff" ) );

		CooldownDurationTag = Manager.AddNativeGameplayTag( TEXT( "Ability.Value.CooldownDuration" ) );
	}

private:
	static FActionRPGGlobalTags ActionRPGTags;

};
