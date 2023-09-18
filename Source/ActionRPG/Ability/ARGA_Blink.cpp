// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_Blink.h"

#include "Ability/AbilityTask/AT_PlayMontagesSequentially.h"
#include "Character/HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "NavigationSystem.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_Blink)

UARGA_Blink::UARGA_Blink( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	StopWhenAbilityEnds = true;
	AnimRootMotionTranslationScale = 1.f;
}

void UARGA_Blink::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );

	if( MontageList.IsEmpty() )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	const auto Hero = Cast<AHeroCharacter>( GetAvatarActorFromActorInfo() );
	float AnimSpeed = 1.f;
	if( Hero )
	{
		if( Hero->GetAttackSpeed() > 0.f )
		{
			AnimSpeed = Hero->GetAttackSpeed();
		}
	}

	ForwardVector = FVector( 0 );
	ForwardVector = Hero->GetLastMovementInputVector();

	UAT_PlayMontagesSequentially* MontagesPlayTask =
		UAT_PlayMontagesSequentially::CreatePlayMontagesSequentiallyProxy( this,
		NAME_None,
		MontageList,
		AnimSpeed,
		StopWhenAbilityEnds,
		AnimRootMotionTranslationScale );

	MontagesPlayTask->OnBlendOut.AddDynamic( this, &UARGA_Blink::OnCompleted );
	MontagesPlayTask->OnCompleted.AddDynamic( this, &UARGA_Blink::OnCompleted );
	MontagesPlayTask->OnCancelled.AddDynamic( this, &UARGA_Blink::OnCancelled );
	MontagesPlayTask->OnInterrupted.AddDynamic( this, &UARGA_Blink::OnCancelled );

	MontagesPlayTask->OnPlayMontage.AddDynamic( this, &UARGA_Blink::OnPlayMontage );

	MontagesPlayTask->ReadyForActivation();
}

void UARGA_Blink::OnPlayMontage( int CurrentIndex )
{
	if( CurrentIndex == 0 )
	{
		return;
	}

	const auto Character = Cast<ACharacter>( GetAvatarActorFromActorInfo() );
	if( Character )
	{
		if( ForwardVector.Length() == 0.f )
		{
			ForwardVector = Character->GetActorForwardVector();
		}

		auto World = GetWorld();
		if( !World )
		{
			return;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem( World );
		if( !NavSystem )
		{
			RLOG( Error, TEXT( "NavSystem is nullptr" ) );
			return;
		}

		float JumpingDistance = Range;
		FVector CurLocation = Character->GetActorLocation();
		FVector MovingVector = ForwardVector * JumpingDistance;
		FVector TargetLocation = CurLocation + MovingVector;
		FNavLocation NavLocation;
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation( CurLocation, TargetLocation );

		while( JumpingDistance > 0.f )
		{
			MovingVector = ForwardVector * JumpingDistance;
			TargetLocation = CurLocation + MovingVector;

			if( NavSystem->ProjectPointToNavigation( TargetLocation, NavLocation ) )
			{
				if( World->FindTeleportSpot( Character, NavLocation.Location, Rotation ) )
				{
#if WITH_EDITOR
					DrawDebugSphere( GetWorld(), NavLocation.Location, 50.f, 16, FColor::Green, false, 0.3f );
#endif
					Character->TeleportTo( NavLocation.Location, Rotation );
					return;
				}
			}

			JumpingDistance -= ( Range / 5.f );
			if( JumpingDistance <= 0.f )
			{
				JumpingDistance = 0.f;
			}
		}
	}
}

void UARGA_Blink::OnCompleted()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UARGA_Blink::OnCancelled()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UARGA_Blink::EndAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled )
{
	if( MovingLockEffectHandle.IsValid() )
	{
		auto ASC = Cast<ABaseCharacter>( GetAvatarActorFromActorInfo() )->GetAbilitySystemComponent();
		if( ASC )
		{
			ASC->RemoveActiveGameplayEffect( MovingLockEffectHandle );
		}
	}

	Super::EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled );
}
