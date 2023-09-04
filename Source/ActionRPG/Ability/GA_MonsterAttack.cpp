// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_MonsterAttack.h"

#include "Character/BaseCharacter.h"
#include "Character/Components/AttackComponent.h"
#include "Character/Components/HitTraceComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Character/Components/ARCharacterStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_MonsterAttack)

UGA_MonsterAttack::UGA_MonsterAttack( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	Rate = 1.f;
	StopWhenAbilityEnd = true;
	AnimRootMotionTranslationScale = 1.f;
	StartTimeSec = 0.f;
}

void UGA_MonsterAttack::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

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

	InitDelegateOnHit();
}

void UGA_MonsterAttack::OnCancelled()
{
	ResetHitTraceComp();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_MonsterAttack::OnCompleted()
{
	ResetHitTraceComp();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_MonsterAttack::OnHit_Implementation( FHitResult HitResult )
{
	const auto Character = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() );
	if( !Character )
	{
		RLOG( Error, TEXT( "Character is null" ) );
		return;
	}

	auto AbilitySystemComponent = Character->GetAbilitySystemComponent();
	if( !AbilitySystemComponent )
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
