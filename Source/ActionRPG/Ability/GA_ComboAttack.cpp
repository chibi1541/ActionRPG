// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_ComboAttack.h"
#include "Character/Components/HitTraceComponent.h"
#include "GameFramework/Character.h"

#include "AbilityTask/AT_PlayMontagesWithGameplayEvent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_ComboAttack)

UGA_ComboAttack::UGA_ComboAttack( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	Rate = 1.f;
	OnlyTriggerOnce = false;
	StopWhenAbilityEnds = true;
	AnimRootMotionTranslationScale = 1.f;
}

void UGA_ComboAttack::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	UAT_PlayMontagesWithGameplayEvent* MontagesPlayTask =
		UAT_PlayMontagesWithGameplayEvent::CreatePlayMontagesWithGameplayEventProxy( this,
		NAME_None,
		AttackMontages,
		EventTag,
		OnlyTriggerOnce,
		Rate,
		StopWhenAbilityEnds,
		AnimRootMotionTranslationScale );


	MontagesPlayTask->OnPlayMontage.AddDynamic( this, &UGA_ComboAttack::SetCurComboIndex );
	MontagesPlayTask->OnNextMontagePlayCheck.BindUFunction( this, FName( "NextAttackAvailable" ) );
	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UGA_ComboAttack::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UGA_ComboAttack::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UGA_ComboAttack::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UGA_ComboAttack::OnCancelled );

	CurComboIndex = 0;
	bNextAttack = false;

	MontagesPlayTask->ReadyForActivation();

	const auto Character = Cast<ACharacter>( GetAvatarActorFromActorInfo() );

	HitTraceComp = Cast<UHitTraceComponent>( Character->GetComponentByClass( UHitTraceComponent::StaticClass() ) );
	if( HitTraceComp == nullptr )
	{
		RLOG( Error, TEXT( "HitTraceComp is null" ) );
		return;
	}
}

void UGA_ComboAttack::InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
{
	if( CurComboIndex + 1 < MaxComboIndex )
	{
		bNextAttack = true;
	}
	else
	{
		bNextAttack = false;
	}
}

void UGA_ComboAttack::OnCancelled()
{
	if( HitTraceComp != nullptr )
	{
		HitTraceComp->ToggleTraceCheck( false );
	}

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_ComboAttack::OnCompleted()
{
	if( HitTraceComp != nullptr )
	{
		HitTraceComp->ToggleTraceCheck( false );
	}

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

bool UGA_ComboAttack::NextAttackAvailable() const
{
	return bNextAttack;
}

void UGA_ComboAttack::SetCurComboIndex( int CurrentIndex )
{
	CurComboIndex = CurrentIndex;

	if( CurComboIndex >= MaxComboIndex )
	{
		CurComboIndex = MaxComboIndex;
	}

	bNextAttack = false;
}
