// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_MeleeAttack.h"

#include "Character/BaseCharacter.h"
#include "Character/Components/HitTraceComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Components/ARCharacterStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_MeleeAttack)

UARGA_MeleeAttack::UARGA_MeleeAttack( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	Rate = 1.f;
	StopWhenAbilityEnd = true;
	AnimRootMotionTranslationScale = 1.f;
	StartTimeSec = 0.f;
}

void UARGA_MeleeAttack::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

	ABaseCharacter* Character = Cast<ABaseCharacter>( ActorInfo->AvatarActor );
	if( Character )
	{
		if( auto HitTrace = Cast<UHitTraceComponent>( Character->GetComponentByClass( UHitTraceComponent::StaticClass() ) ) )
		{
			if( auto Ability = Cast<UARGA_MeleeAttack>( Spec.GetPrimaryInstance() ) )
			{
				Ability->HitTraceComp = HitTrace;

				RLOG( Warning, TEXT( "%s" ), *HitTrace->GetName() );
			}
		}
	}
}

void UARGA_MeleeAttack::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );

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

	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UARGA_MeleeAttack::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UARGA_MeleeAttack::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UARGA_MeleeAttack::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UARGA_MeleeAttack::OnCancelled );

	MontagesPlayTask->ReadyForActivation();

	InitDelegateOnHit_Implementation();
}

void UARGA_MeleeAttack::OnCancelled()
{
	ResetHitTraceComp_Implementation();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UARGA_MeleeAttack::OnCompleted()
{
	ResetHitTraceComp_Implementation();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UARGA_MeleeAttack::OnHit_Implementation( FHitResult HitResult )
{
	const auto Character = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() );
	if( !Character )
	{
		RLOG( Error, TEXT( "Character is null" ) );
		return;
	}

	if( !AbilitySystemComponent.IsValid() )
	{
		RLOG( Error, TEXT( "AbilitySystemComponent is null" ) );
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject( this );
	EffectContext.AddHitResult( HitResult, true );
	if( DamageCalcEffect )
	{
		FGameplayEffectSpecHandle DamageSpecHandle = AbilitySystemComponent->MakeOutgoingSpec( DamageCalcEffect, Character->GetCharacterLevel(), EffectContext );
		if( DamageSpecHandle.IsValid() )
		{
			auto TargetASC = Cast<ABaseCharacter>( HitResult.GetActor() )->GetAbilitySystemComponent();
			if( !TargetASC )
			{
				RLOG( Error, TEXT( "Target ASC is null" ) );
				return;
			}

			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *DamageSpecHandle.Data.Get(), TargetASC );
		}
	}

	if( StiffEffect )
	{
		FGameplayEffectSpecHandle StiffSpecHandle = AbilitySystemComponent->MakeOutgoingSpec( StiffEffect, Character->GetCharacterLevel(), EffectContext );
		if( StiffSpecHandle.IsValid() )
		{
			auto CharacterStateComp = Cast<ABaseCharacter>( HitResult.GetActor() )->GetCharacterStateComponenet();
			if( !CharacterStateComp )
			{
				RLOG( Error, TEXT( "Target's CharacterStateComp is null" ) );
				return;
			}

			CharacterStateComp->SetStiffEffectSpec( StiffSpecHandle );

		}

	}
}

