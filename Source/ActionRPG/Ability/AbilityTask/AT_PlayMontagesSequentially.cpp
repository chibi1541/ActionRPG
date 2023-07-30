// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask/AT_PlayMontagesSequentially.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "AbilitySystemLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_PlayMontagesSequentially)

UAT_PlayMontagesSequentially::UAT_PlayMontagesSequentially( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

UAT_PlayMontagesSequentially* UAT_PlayMontagesSequentially::CreatePlayMontagesSequentiallyProxy( UGameplayAbility* OwningAbility, FName TaskInstanceName, TArray<UAnimMontage*> MontageList, float Rate /*= 1.f*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f */ )
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate( Rate );

	UAT_PlayMontagesSequentially* NewTask = NewAbilityTask<UAT_PlayMontagesSequentially>( OwningAbility, TaskInstanceName );
	NewTask->PlayMontageList = MontageList;
	NewTask->Rate = Rate;
	NewTask->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	NewTask->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return NewTask;
}

UAT_PlayMontagesSequentially* UAT_PlayMontagesSequentially::K2_CreatePlayMontagesSequentiallyProxy( UGameplayAbility* OwningAbility, FName TaskInstanceName, TArray<UAnimMontage*> MontageList, float Rate /*= 1.f*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f */ )
{
	return CreatePlayMontagesSequentiallyProxy( OwningAbility, TaskInstanceName, MontageList, Rate, bStopWhenAbilityEnds, AnimRootMotionTranslationScale );
}

void UAT_PlayMontagesSequentially::OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted )
{
	if( CanPlayNextMontage() && !bInterrupted )
	{
		if( PlayNextMontage() == false )
		{
			EndTask();
		}

		return;
	}

	if( Ability && Ability->GetCurrentMontage() == PlayMontageList[MontagePlayIndex] )
	{
		if( Montage == PlayMontageList[MontagePlayIndex] )
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

void UAT_PlayMontagesSequentially::OnMontageInterrupted()
{
	if( StopPlayingMontage() == true )
	{
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnInterrupted.Broadcast();
		}
	}

}

void UAT_PlayMontagesSequentially::OnMontageEnded( UAnimMontage* Montage, bool bInterrupted )
{
	if( Montage == PlayMontageList[MontagePlayIndex] && !bInterrupted )
	{
		if( ShouldBroadcastAbilityTaskDelegates() )
		{
			OnCompleted.Broadcast();
		}
	}
}

void UAT_PlayMontagesSequentially::Activate()
{
	if( Ability == nullptr )
	{
		return;
	}

	MontagePlayIndex = 0;
	bool bPlayedMontage = false;
	if( UAbilitySystemComponent* ASC = AbilitySystemComponent.Get() )
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

				BlendingOutDelegate.BindUObject( this, &UAT_PlayMontagesSequentially::OnMontageBlendingOut );
				AnimInstance->Montage_SetBlendingOutDelegate( BlendingOutDelegate, PlayMontageList[MontagePlayIndex] );

				MontageEndedDelegate.BindUObject( this, &UAT_PlayMontagesSequentially::OnMontageEnded );
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
			ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesSequentially call to PlayMontage failed!" ) );
		}
	}
	else
	{
		ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesSequentially called on invalid AbilitySystemComponent" ) );
	}

	if( bPlayedMontage )
	{
		InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject( this, &UAT_PlayMontagesSequentially::OnMontageInterrupted );
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

bool UAT_PlayMontagesSequentially::PlayNextMontage()
{
	bool bPlayMontage = false;

	MontagePlayIndex++;
	if( MontagePlayIndex >= PlayMontageList.Num() )
	{
		return false;
	}

	if( UAbilitySystemComponent* ASC = AbilitySystemComponent.Get() )
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if( AnimInstance != nullptr )
		{
			if( ASC->PlayMontage( Ability, Ability->GetCurrentActivationInfo(), PlayMontageList[MontagePlayIndex], Rate, NAME_None, 0.f ) > 0.f )
			{
				if( ShouldBroadcastAbilityTaskDelegates() == true )
				{
					BlendingOutDelegate.BindUObject( this, &UAT_PlayMontagesSequentially::OnMontageBlendingOut );
					AnimInstance->Montage_SetBlendingOutDelegate( BlendingOutDelegate, PlayMontageList[MontagePlayIndex] );

					MontageEndedDelegate.BindUObject( this, &UAT_PlayMontagesSequentially::OnMontageEnded );
					AnimInstance->Montage_SetEndDelegate( MontageEndedDelegate, PlayMontageList[MontagePlayIndex] );

					if( OnPlayMontage.IsBound() )
					{
						OnPlayMontage.Broadcast( MontagePlayIndex );
					}

					bPlayMontage = true;
				}
			}
		}
		else
		{
			ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesSequentially call to NextPlayMontage failed!" ) );
		}
	}
	else
	{
		ABILITY_LOG( Warning, TEXT( "UAT_PlayMontagesSequentially called on invalid AbilitySystemComponent" ) );
	}

	return bPlayMontage;
}


bool UAT_PlayMontagesSequentially::CanPlayNextMontage()
{
	if( OnNextMontagePlayCheck.IsBound() && OnNextMontagePlayCheck.Execute() == false )
	{
		return false;
	}

	if( MontagePlayIndex >= ( PlayMontageList.Num() - 1 ) )
	{
		return false;
	}

	return true;
}

void UAT_PlayMontagesSequentially::ExternalCancel()
{
	if( ShouldBroadcastAbilityTaskDelegates() )
	{
		OnCancelled.Broadcast();
	}

	Super::ExternalCancel();
}

void UAT_PlayMontagesSequentially::OnDestroy( bool AbilityEnded )
{
	if( Ability )
	{
		Ability->OnGameplayAbilityCancelled.Remove( InterruptedHandle );
		if( AbilityEnded && bStopWhenAbilityEnds )
		{
			StopPlayingMontage();
		}
	}

	Super::OnDestroy( AbilityEnded );
}

bool UAT_PlayMontagesSequentially::StopPlayingMontage()
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

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if( ASC && Ability )
	{
		if( ASC->GetAnimatingAbility() == Ability
			&& ASC->GetCurrentMontage() == PlayMontageList[MontagePlayIndex] )
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage( PlayMontageList[MontagePlayIndex] );
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

bool UAT_PlayMontagesSequentially::IsContainMontage( UAnimMontage* AnimMontage )
{
	for( const UAnimMontage* Montage : PlayMontageList )
	{
		if( Montage == AnimMontage )
		{
			return true;
		}
	}

	return false;
}

