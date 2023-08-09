// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_MonsterAttack.h"

#include "Character/BaseCharacter.h"
#include "Character/Components/AttackComponent.h"
#include "Character/Components/HitTraceComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_MonsterAttack)

UGA_MonsterAttack::UGA_MonsterAttack( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	Rate = 1.f;
	StopWhenAbilityEnd = true;
	AnimRootMotionTranslationScale = 1.f;
	StartTimeSec = 0.f;
}

void UGA_MonsterAttack::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	UAttackComponent* AttackComp = Cast<UAttackComponent>( ActorInfo->AvatarActor->GetComponentByClass( UAttackComponent::StaticClass() ) );
	if( AttackComp != nullptr )
	{
		if( auto AttackAbility = Cast<UGA_MonsterAttack>( Spec.GetPrimaryInstance() ) )
		{
			AttackAbility->AttackMontages = AttackComp->GetAttackMontages();

			RLOG( Warning, TEXT( "%s, %d" ), *AttackAbility->GetName(), AttackMontages.Num() );
		}
	}
}

void UGA_MonsterAttack::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	if( AttackMontages.IsEmpty() )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	int AnimIndex = FMath::RandRange( 0, AttackMontages.Num() - 1 );

	UAbilityTask_PlayMontageAndWait* MontagesPlayTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy( this,
		NAME_None,
		AttackMontages[AnimIndex],
		Rate,
		NAME_None,
		StopWhenAbilityEnd,
		AnimRootMotionTranslationScale,
		StartTimeSec );

	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UGA_MonsterAttack::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UGA_MonsterAttack::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UGA_MonsterAttack::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UGA_MonsterAttack::OnCancelled );

	MontagesPlayTask->ReadyForActivation();

	const auto Character = Cast<ACharacter>( GetAvatarActorFromActorInfo() );
	HitTraceComp = Cast<UHitTraceComponent>( Character->GetComponentByClass( UHitTraceComponent::StaticClass() ) );
	if( HitTraceComp == nullptr )
	{
		RLOG( Error, TEXT( "HitTraceComp is null" ) );
		return;
	}
}

void UGA_MonsterAttack::OnCancelled()
{
	if( HitTraceComp != nullptr )
	{
		HitTraceComp->ToggleTraceCheck( false );
	}

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_MonsterAttack::OnCompleted()
{
	if( HitTraceComp != nullptr )
	{
		HitTraceComp->ToggleTraceCheck( false );
	}

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

