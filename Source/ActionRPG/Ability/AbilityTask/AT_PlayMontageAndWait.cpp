// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask/AT_PlayMontageAndWait.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_PlayMontageAndWait)


UAT_PlayMontageAndWait::UAT_PlayMontageAndWait( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

void UAT_PlayMontageAndWait::OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted )
{
	if( Ability && Ability->GetCurrentMontage() == MontageToPlay )
	{
		if( Montage == MontageToPlay )
		{
			if( UAbilitySystemComponent* ASC = AbilitySystemComponent.Get() )
			{
				ASC->ClearAnimatingAbility( Ability );
			}

			ACharacter* Character = Cast<ACharacter>( GetAvatarActor() );
			if( Character && ( Character->GetLocalRole() == ROLE_Authority ||
				( Character->GetLocalRole() == ROLE_AutonomousProxy &&
				Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted ) ) )
			{
				Character->SetAnimRootMotionTranslationScale( 1.f );
			}

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

void UAT_PlayMontageAndWait::OnMontageInterrupted()
{
	if( StopPlayingMontage() )
	{
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnInterrupted.Broadcast();
		}
	}
}

void UAT_PlayMontageAndWait::OnMontageEnded( UAnimMontage* Montage, bool bInterrupted )
{
	if( bInterrupted == false )
	{
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnCompleted.Broadcast();
		}
	}

	EndTask();
}

UAT_PlayMontageAndWait* UAT_PlayMontageAndWait::CreatePlayMontageAndWait( UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate /*= 1.f*/, FName StartSeciton, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f */ )
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate( Rate );

	UAT_PlayMontageAndWait* MyObj = NewAbilityTask<UAT_PlayMontageAndWait>( OwningAbility, TaskInstanceName );
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSeciton;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UAT_PlayMontageAndWait::Activate()
{
	if( Ability == nullptr )
	{
		return;
	}

	bool bPlayedMontage = false;

	if( UAbilitySystemComponent* ASC = AbilitySystemComponent.Get() )
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if( AnimInstance != nullptr )
		{
			if( ASC->PlayMontage( Ability, Ability->GetCurrentActivationInfo(),
				MontageToPlay, Rate, StartSection, StartTimeSeconds ) > 0.f )
			{
				if( ShouldBroadcastAbilityTaskDelegates() == false )
				{
					return;
				}

				InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject( this, &UAT_PlayMontageAndWait::OnMontageInterrupted );

				BlendingOutDelegate.BindUObject( this, &UAT_PlayMontageAndWait::OnMontageBlendingOut );
				AnimInstance->Montage_SetBlendingOutDelegate( BlendingOutDelegate, MontageToPlay );

				MontageEndedDelegate.BindUObject( this, &UAT_PlayMontageAndWait::OnMontageEnded );
				AnimInstance->Montage_SetEndDelegate( MontageEndedDelegate, MontageToPlay );

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
			ABILITY_LOG( Warning, TEXT( "AT_PlayMontageAndWait call to PlayMontage failed!" ) );
		}
	}
	else
	{
		ABILITY_LOG( Warning, TEXT( "AT_PlayMontageAndWait called on invalid AbilitySystemComponent" ) );
	}

	if( !bPlayedMontage )
	{
		ABILITY_LOG( Warning, TEXT( "AT_PlayMontageAndWait called in Ability %s failed to play montage %s; Task Instance Name %s." ), *Ability->GetName(), *GetNameSafe( MontageToPlay ), *InstanceName.ToString() );
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnCancelled.Broadcast();
		}
	}

	SetWaitingOnAvatar();
}

void UAT_PlayMontageAndWait::ExternalCancel()
{
	if( ShouldBroadcastAbilityTaskDelegates() == true )
	{
		OnCancelled.Broadcast();
	}

	Super::ExternalCancel();
}

void UAT_PlayMontageAndWait::OnDestroy( bool bInOwnerFinished )
{
	if( Ability )
	{
		Ability->OnGameplayAbilityCancelled.Remove( InterruptedHandle );
		if( bInOwnerFinished && bStopWhenAbilityEnds )
		{
			StopPlayingMontage();
		}
	}

	Super::OnDestroy( bInOwnerFinished );
}

bool UAT_PlayMontageAndWait::StopPlayingMontage()
{
	if( Ability == nullptr )
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if( ActorInfo == nullptr )
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if( AnimInstance == nullptr )
	{
		return false;
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if( ASC && Ability )
	{
		if( ASC->GetAnimatingAbility() == Ability &&
			ASC->GetCurrentMontage() == MontageToPlay )
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage( MontageToPlay );
			if( MontageInstance )
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;

}
