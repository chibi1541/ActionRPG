// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_Casting.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_Casting)

void UARGA_Casting::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	if( CheckCost( Handle, ActorInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
		return;
	}

	float StartTimeSeconds = CastingMontage->GetPlayLength() - CastingTime;

	if( TriggerEventData )
	{
		CurrentEventData = *TriggerEventData;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, CastingMontage, 1.0f, NAME_None,
		true, 1.0f, StartTimeSeconds );

	PlayMontageTask->OnCompleted.AddDynamic( this, &UARGA_Casting::OnCompleted );
	PlayMontageTask->OnBlendOut.AddDynamic( this, &UARGA_Casting::OnCompleted );
	PlayMontageTask->OnCancelled.AddDynamic( this, &UARGA_Casting::OnCancelled );
	PlayMontageTask->OnInterrupted.AddDynamic( this, &UARGA_Casting::OnCancelled );

	PlayMontageTask->ReadyForActivation();
}

void UARGA_Casting::OnCompleted()
{
	RLOG( Warning, TEXT( "Casting Over" ) );

	TriggerAbility();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UARGA_Casting::OnCancelled()
{
	RLOG( Warning, TEXT( "Casting Interrupted" ) );

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}
