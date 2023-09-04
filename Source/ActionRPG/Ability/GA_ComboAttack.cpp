// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_ComboAttack.h"

#include "Character/Components/HitTraceComponent.h"
#include "Character/Components/AttackComponent.h"
#include "Character/HeroCharacter.h"
#include "AbilitySystemComponent.h"
#include "Character/Components/ARCharacterStateComponent.h"

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

void UGA_ComboAttack::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

	UAttackComponent* AttackComp = Cast<UAttackComponent>( ActorInfo->AvatarActor->GetComponentByClass( UAttackComponent::StaticClass() ) );
	if( AttackComp != nullptr )
	{
		if( auto AttackAbility = Cast<UGA_ComboAttack>( Spec.GetPrimaryInstance() ) )
		{
			AttackAbility->AttackMontages = AttackComp->GetAttackMontages();
			MaxComboIndex = AttackAbility->AttackMontages.Num();

			RLOG( Warning, TEXT( "%s, %d" ), *AttackAbility->GetName(), AttackMontages.Num() );
		}
	}
}

void UGA_ComboAttack::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	const auto Hero = Cast<AHeroCharacter>( GetAvatarActorFromActorInfo() );
	float AttackSpeed = 1.f;
	if( Hero )
	{
		if( Hero->GetAttackSpeed() > 0.f )
		{
			AttackSpeed = Hero->GetAttackSpeed();
		}
	}

	UAT_PlayMontagesWithGameplayEvent* MontagesPlayTask =
		UAT_PlayMontagesWithGameplayEvent::CreatePlayMontagesWithGameplayEventProxy( this,
		NAME_None,
		AttackMontages,
		EventTag,
		OnlyTriggerOnce,
		AttackSpeed,
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

	InitDelegateOnHit();
}

void UGA_ComboAttack::InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
{
	RLOG( Warning, TEXT( "Call Pressed Input Func" ) );

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
	ResetHitTraceComp();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_ComboAttack::OnCompleted()
{
	ResetHitTraceComp();

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

void UGA_ComboAttack::OnHit_Implementation( FHitResult HitResult )
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
		FGameplayEffectSpecHandle DamageHandle = AbilitySystemComponent->MakeOutgoingSpec( DamageCalcEffect, Character->GetCharacterLevel(), EffectContext );
		if( DamageHandle.IsValid() )
		{
			auto TargetASC = Cast<ABaseCharacter>( HitResult.GetActor() )->GetAbilitySystemComponent();
			if( !TargetASC )
			{
				RLOG( Error, TEXT( "Target ASC is null" ) );
				return;
			}

			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *DamageHandle.Data.Get(), TargetASC );
		}
	}

	if( StiffEffect )
	{
		FGameplayEffectSpecHandle StiffHandle = AbilitySystemComponent->MakeOutgoingSpec( StiffEffect, Character->GetCharacterLevel(), EffectContext );
		if( StiffHandle.IsValid() )
		{
			auto CharacterStateComp = Cast<ABaseCharacter>( HitResult.GetActor() )->GetCharacterStateComponenet();
			if( !CharacterStateComp )
			{
				RLOG( Error, TEXT( "Target's CharacterStateComp is null" ) );
				return;
			}

			CharacterStateComp->SetStiffEffectSpec( StiffHandle );
		}
	}

}