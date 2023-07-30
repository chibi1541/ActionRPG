// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask/AT_PlayMontagesWithGameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemLog.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_PlayMontagesWithGameplayEvent)

UAT_PlayMontagesWithGameplayEvent::UAT_PlayMontagesWithGameplayEvent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	bOnlyTriggerOnce = false;
}

UAT_PlayMontagesWithGameplayEvent* UAT_PlayMontagesWithGameplayEvent::CreatePlayMontagesWithGameplayEventProxy( UGameplayAbility* OwningAbility, FName TaskInstanceName, TArray<UAnimMontage*> MontageList, FGameplayTag EventTag, bool OnlyTriggerOnce /*= false*/, float AnimRate /*= 1.f*/, bool StopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f */ )
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate( AnimRate );

	UAT_PlayMontagesWithGameplayEvent* NewTask = NewAbilityTask<UAT_PlayMontagesWithGameplayEvent>( OwningAbility, TaskInstanceName );
	NewTask->PlayMontageList = MontageList;
	NewTask->Rate = AnimRate;
	NewTask->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	NewTask->bStopWhenAbilityEnds = StopWhenAbilityEnds;
	NewTask->CheckEventTag = EventTag;
	NewTask->bOnlyTriggerOnce = OnlyTriggerOnce;

	return NewTask;
}

UAT_PlayMontagesWithGameplayEvent* UAT_PlayMontagesWithGameplayEvent::K2_CreatePlayMontagesWithGameplayEventProxy( UGameplayAbility* OwningAbility, FName TaskInstanceName, TArray<UAnimMontage*> MontageList, FGameplayTag EventTag, bool OnlyTriggerOnce /*= false*/, float AnimRate /*= 1.f*/, bool StopWhenAbilityEnds /*= true*/, float fAnimRootMotionTranslationScale /*= 1.f */ )
{
	return CreatePlayMontagesWithGameplayEventProxy( OwningAbility, TaskInstanceName, MontageList, EventTag, OnlyTriggerOnce, AnimRate,StopWhenAbilityEnds, fAnimRootMotionTranslationScale );
}


void UAT_PlayMontagesWithGameplayEvent::OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted )
{
	if( Ability && Ability->GetCurrentMontage() == Montage )
	{
		if( Montage == PlayMontageList[MontagePlayIndex] )
		{
			if( UAbilitySystemComponent* ASC = AbilitySystemComponent.Get() )
			{
				ASC->ClearAnimatingAbility( Ability );
			}

			ACharacter* Character = Cast<ACharacter>( GetAvatarActor() );
			if( Character && ( Character->GetLocalRole() == ROLE_Authority ||
				( Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted ) ) )
			{
				Character->SetAnimRootMotionTranslationScale( 1.f );
			}
		}

		if( bInterrupted )
		{
			if( ShouldBroadcastAbilityTaskDelegates() )
			{
				OnInterrupted.Broadcast();
			}
		}
		else
		{
			if( ShouldBroadcastAbilityTaskDelegates() )
			{
				OnBlendOut.Broadcast();
			}
		}
	}
}

void UAT_PlayMontagesWithGameplayEvent::Activate()
{
	if( Ability == nullptr )
	{
		return;
	}

	MontagePlayIndex = 0;
	bool bPlayedMontage = false;
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if( ASC )
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		bool bCanPlayMontageList = PlayMontageList.Num() > MontagePlayIndex;

		if( AnimInstance != nullptr && bCanPlayMontageList == true )
		{
			if( ASC->PlayMontage( Ability, Ability->GetCurrentActivationInfo(), PlayMontageList[MontagePlayIndex], Rate, NAME_None, 0.f ) > 0.f )
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if( ShouldBroadcastAbilityTaskDelegates() == false )
				{
					return;
				}

				BlendingOutDelegate.BindUObject( this, &UAT_PlayMontagesWithGameplayEvent::OnMontageBlendingOut );
				AnimInstance->Montage_SetBlendingOutDelegate( BlendingOutDelegate, PlayMontageList[MontagePlayIndex] );

				MontageEndedDelegate.BindUObject( this, &UAT_PlayMontagesWithGameplayEvent::OnMontageEnded );
				AnimInstance->Montage_SetEndDelegate( MontageEndedDelegate, PlayMontageList[MontagePlayIndex] );

				ACharacter* Character = Cast<ACharacter>( GetAvatarActor() );
				if( Character && ( Character->GetLocalRole() == ROLE_Authority ||
					( Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted ) ) )
				{
					Character->SetAnimRootMotionTranslationScale( AnimRootMotionTranslationScale );
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesWithGameplayEvent call to PlayMontage failed!" ) );
		}
	}
	else
	{
		ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesWithGameplayEvent called on invalid AbilitySystemComponent" ) );
	}

	if( bPlayedMontage )
	{
		InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject( this, &UAT_PlayMontagesWithGameplayEvent::OnMontageInterrupted );
		CheckEventHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd( CheckEventTag ).AddUObject( this, &UAT_PlayMontagesWithGameplayEvent::GameplayEventCallback );
		if( OnPlayMontage.IsBound() )
		{
			OnPlayMontage.Broadcast( MontagePlayIndex );
		}
	}
	else
	{
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnCancelled.Broadcast();
		}
	}

	SetWaitingOnAvatar();
}

void UAT_PlayMontagesWithGameplayEvent::GameplayEventCallback( const FGameplayEventData* Payload )
{
	if( CanPlayNextMontage() == true )
	{
		if( PlayNextMontage() == false )
		{
			EndTask();
		}
	}

	if( bOnlyTriggerOnce )
	{
		UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
		if( ASC && CheckEventHandle.IsValid() )
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd( CheckEventTag ).Remove( CheckEventHandle );
		}
	}
}

void UAT_PlayMontagesWithGameplayEvent::OnDestroy( bool AbilityEnded )
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if( ASC && CheckEventHandle.IsValid() )
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd( CheckEventTag ).Remove( CheckEventHandle );
	}

	Super::OnDestroy( AbilityEnded );
}
