// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_ComboAttackWithProjectile.h"

#include "AbilityTask/AT_PlayMontagesWithGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/HeroCharacter.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/ARProjectile.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_ComboAttackWithProjectile)

UARGA_ComboAttackWithProjectile::UARGA_ComboAttackWithProjectile( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	OnlyTriggerOnce = false;
	StopWhenAbilityEnds = true;
	AnimRootMotionTranslationScale = 1.f;

	CurComboIndex = 0;
	bNextAttack = false;
}

void UARGA_ComboAttackWithProjectile::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

	if( auto AttackAbility = Cast<UARGA_ComboAttackWithProjectile>( Spec.GetPrimaryInstance() ) )
	{
		if( !ComboAttackDatas.IsEmpty() )
		{
			ComboAttackDatas.Sort( [] (
				const FAttackComboData_Projectile& LValue, const FAttackComboData_Projectile& RValue )
			{
				return LValue.ComboIndex < RValue.ComboIndex;
			} );
		}
	}
}

void UARGA_ComboAttackWithProjectile::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
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

	MontagesPlayTask->OnPlayMontage.AddDynamic( this, &UARGA_ComboAttackWithProjectile::OnPlayMontage );
	MontagesPlayTask->OnNextMontagePlayCheck.BindUFunction( this, FName( "NextAttackAvailable" ) );
	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UARGA_ComboAttackWithProjectile::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UARGA_ComboAttackWithProjectile::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UARGA_ComboAttackWithProjectile::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UARGA_ComboAttackWithProjectile::OnCancelled );

	CurComboIndex = 0;
	bNextAttack = false;

	MontagesPlayTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* EventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent( this,
		ProjectileFireTag );

	EventTask->EventReceived.AddDynamic( this, &UARGA_ComboAttackWithProjectile::ProjectileFire );

	EventTask->ReadyForActivation();
}

void UARGA_ComboAttackWithProjectile::OnCompleted()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UARGA_ComboAttackWithProjectile::OnCancelled()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}


void UARGA_ComboAttackWithProjectile::InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
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

bool UARGA_ComboAttackWithProjectile::SpendStamina()
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

void UARGA_ComboAttackWithProjectile::ProjectileFire( FGameplayEventData Payload )
{
	if( CurComboIndex >= ComboAttackDatas.Num() )
	{
		EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
	}

	AHeroCharacter* Hero = Cast<AHeroCharacter>( GetAvatarActorFromActorInfo() );
	if( !Hero )
	{
		EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
	}

	const FVector Forward = Hero->GetActorForwardVector();
	const FVector HeroLocation = Hero->GetActorLocation();

	FVector Start = Hero->FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation( ComboAttackDatas[CurComboIndex].StartSocketName );
	FVector End = FVector( HeroLocation.X, HeroLocation.Y, Start.Z ) + Forward * Range;
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation( Start, End );

	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec( DamageCalcEffect, GetAbilityLevel() );
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	DamageEffectSpecHandle.Data->SetSetByCallerMagnitude( Tags.ExtraDamageTag, ComboAttackDatas[CurComboIndex].ExtraDamage );

	FGameplayEffectSpecHandle StiffSpecHandle = MakeOutgoingGameplayEffectSpec( StiffEffect, GetAbilityLevel() );

	FTransform MuzzleTransform = Hero->FindComponentByClass<USkeletalMeshComponent>()->GetSocketTransform( ComboAttackDatas[CurComboIndex].StartSocketName );
	MuzzleTransform.SetRotation( Rotation.Quaternion() );
	MuzzleTransform.SetScale3D( FVector( 1.0f ) );

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AARProjectile* Projectile = GetWorld()->SpawnActorDeferred<AARProjectile>( ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),
		Hero, ESpawnActorCollisionHandlingMethod::AlwaysSpawn );
	Projectile->DamageCalcEffectSpecHandle = DamageEffectSpecHandle;
	Projectile->StiffEffectSpecHandle = StiffSpecHandle;
	Projectile->Range = Range;
	Projectile->Speed = Speed;

	Projectile->FinishSpawning( MuzzleTransform );
}

void UARGA_ComboAttackWithProjectile::OnPlayMontage( int CurrentIndex )
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

bool UARGA_ComboAttackWithProjectile::NextAttackAvailable() const
{
	return bNextAttack;
}
