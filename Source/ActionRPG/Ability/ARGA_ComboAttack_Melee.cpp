// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_ComboAttack_Melee.h"

#include "Character/HeroCharacter.h"
#include "Character/Components/HitTraceComponent.h"
#include "AbilityTask/AT_PlayMontagesWithGameplayEvent.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Components/ARCharacterStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_ComboAttack_Melee)

UARGA_ComboAttack_Melee::UARGA_ComboAttack_Melee( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	OnlyTriggerOnce = false;
	StopWhenAbilityEnds = true;
	AnimRootMotionTranslationScale = 1.f;
}

void UARGA_ComboAttack_Melee::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

	ABaseCharacter* Character = Cast<ABaseCharacter>( ActorInfo->AvatarActor );
	if( Character )
	{
		if( auto HitTrace = Cast<UHitTraceComponent>( Character->GetComponentByClass( UHitTraceComponent::StaticClass() ) ) )
		{
			if( auto Ability = Cast<UARGA_ComboAttack_Melee>( Spec.GetPrimaryInstance() ) )
			{
				Ability->HitTraceComp = HitTrace;
			}
		}
	}

	if( auto AttackAbility = Cast<UARGA_ComboAttack_Melee>( Spec.GetPrimaryInstance() ) )
	{
		if( !ComboAttackDatas.IsEmpty() )
		{
			ComboAttackDatas.Sort( [] (
				const FAttackComboData& LValue, const FAttackComboData& RValue )
			{
				return LValue.ComboIndex < RValue.ComboIndex;
			} );
		}
	}
}

void UARGA_ComboAttack_Melee::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	CurComboIndex = 0;

	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );

	if( ComboAttackDatas.IsEmpty() )
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

	TArray<UAnimMontage*> AttackMontages;
	AttackMontages.Empty();

	for( auto Data : ComboAttackDatas )
	{
		AttackMontages.Add( Data.AttackMontage );
	}

	UAT_PlayMontagesWithGameplayEvent* MontagesPlayTask =
		UAT_PlayMontagesWithGameplayEvent::CreatePlayMontagesWithGameplayEventProxy( this,
		NAME_None,
		AttackMontages,
		NextAttackTag,
		OnlyTriggerOnce,
		AttackSpeed,
		StopWhenAbilityEnds,
		AnimRootMotionTranslationScale );

	MontagesPlayTask->OnPlayMontage.AddDynamic( this, &UARGA_ComboAttack_Melee::OnPlayMontage );
	MontagesPlayTask->OnNextMontagePlayCheck.BindUFunction( this, FName( "NextAttackAvailable" ) );
	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UARGA_ComboAttack_Melee::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UARGA_ComboAttack_Melee::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UARGA_ComboAttack_Melee::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UARGA_ComboAttack_Melee::OnCancelled );

	CurComboIndex = 0;
	bNextAttack = false;

	MontagesPlayTask->ReadyForActivation();

	InitDelegateOnHit_Implementation();
}

void UARGA_ComboAttack_Melee::InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
{
	if( CurComboIndex + 1 < ComboAttackDatas.Num() )
	{
		if( !IsExhausted() )
		{
			bNextAttack = true;
			return;
		}
	}

	bNextAttack = false;
}


bool UARGA_ComboAttack_Melee::SpendStamina()
{
	if( CurComboIndex >= ComboAttackDatas.Num() )
	{
		return false;
	}

	if( !AbilitySystemComponent.IsValid() )
	{
		const auto Character = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() );
		if( Character )
		{
			AbilitySystemComponent = Character->GetAbilitySystemComponent();
		}
	}

	if( !AbilitySystemComponent.IsValid() )
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	if( StaminaSpandEffect )
	{
		FGameplayEffectSpecHandle StaminaCostHandle = AbilitySystemComponent->MakeOutgoingSpec( StaminaSpandEffect, 1, EffectContext );
		if( StaminaCostHandle.IsValid() )
		{
			const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

			int ModifiedCost = StaminaCost + ComboAttackDatas[CurComboIndex].ExtraStaminaCost;

			StaminaCostHandle.Data->SetSetByCallerMagnitude( Tags.AbilityCostTag_Stamina, ModifiedCost );

			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *StaminaCostHandle.Data.Get(), AbilitySystemComponent.Get() );

			return true;
		}
	}

	return false;
}

void UARGA_ComboAttack_Melee::OnCompleted()
{
	ResetHitTraceComp_Implementation();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UARGA_ComboAttack_Melee::OnCancelled()
{
	ResetHitTraceComp_Implementation();

	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UARGA_ComboAttack_Melee::OnPlayMontage( int CurrentIndex )
{
	// Update Combo Index
	CurComboIndex = CurrentIndex;
	if( CurComboIndex >= ComboAttackDatas.Num() )
	{
		CurComboIndex = ComboAttackDatas.Num() - 1;
	}

	bNextAttack = false;

	if( CurComboIndex == 0 )
	{
		return;
	}

	// SpendStamina
	SpendStamina();
}

bool UARGA_ComboAttack_Melee::NextAttackAvailable() const
{
	return bNextAttack;
}

void UARGA_ComboAttack_Melee::OnHit_Implementation( FHitResult HitResult )
{
	if( CurComboIndex >= ComboAttackDatas.Num() )
	{
		return;
	}

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
		FGameplayEffectSpecHandle DamageHandle = AbilitySystemComponent->MakeOutgoingSpec( DamageCalcEffect, Character->GetCharacterLevel(), EffectContext );
		if( DamageHandle.IsValid() )
		{
			auto TargetASC = Cast<ABaseCharacter>( HitResult.GetActor() )->GetAbilitySystemComponent();
			if( !TargetASC )
			{
				RLOG( Error, TEXT( "Target ASC is null" ) );
				return;
			}

			const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
			DamageHandle.Data->SetSetByCallerMagnitude( Tags.ExtraDamageTag, ComboAttackDatas[CurComboIndex].ExtraDamage );

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

