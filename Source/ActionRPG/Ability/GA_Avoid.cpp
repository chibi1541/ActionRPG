// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_Avoid.h"

#include "Ability/AbilityTask/AT_PlayMontageAndWait.h"
#include "Ability/AbilityTask/AT_ApplyRootMotionConstantForce.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/RootMotionSource.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_Avoid)

UGA_Avoid::UGA_Avoid( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag( FGameplayTag::RequestGameplayTag( FName( "Ability.Action.Avoid" ) ) );
}

void UGA_Avoid::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData )
{

	if( CommitAbility( Handle, ActorInfo, ActivationInfo ) == false )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, false );
		return;
	}

	// 플레이어 일 경우
	const APawn* OwnerPawn = Cast<APawn>( ActorInfo->AvatarActor );
	if( OwnerPawn == nullptr )
	{
		EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
		return;
	}

	// Play AnimMontage
	UAT_PlayMontageAndWait* PlayMontageTask = UAT_PlayMontageAndWait::CreatePlayMontageAndWait( this, NAME_None, MontageToPlay, 1.3f );
	PlayMontageTask->OnBlendOut.AddDynamic( this, &UGA_Avoid::OnCompleted );
	PlayMontageTask->OnCompleted.AddDynamic( this, &UGA_Avoid::OnCompleted );
	PlayMontageTask->OnCancelled.AddDynamic( this, &UGA_Avoid::OnCancelled );
	PlayMontageTask->OnInterrupted.AddDynamic( this, &UGA_Avoid::OnCancelled );

	PlayMontageTask->ReadyForActivation();

	InputDir = OwnerPawn->GetLastMovementInputVector();
	ForwardDir = OwnerPawn->GetActorForwardVector();

	FVector WorldDirection = FVector::ZeroVector;

	if( InputDir.Length() != 0.f )
	{
		WorldDirection = InputDir;
	}
	else
	{
		WorldDirection = ForwardDir;
	}

	FRotator DesiredRotation = WorldDirection.Rotation();
	DesiredRotation.Pitch = 0.0f;
	DesiredRotation.Yaw = FRotator::NormalizeAxis( DesiredRotation.Yaw );
	DesiredRotation.Roll = 0.0f;

	ActorInfo->AvatarActor->SetActorRotation( DesiredRotation );

	UAT_ApplyRootMotionConstantForce* RootMotionTask =
		UAT_ApplyRootMotionConstantForce::CreateApplyRootMotionConstantForce( this,
		NAME_None,
		WorldDirection,
		Strength,
		Duration,
		false,
		nullptr,
		ERootMotionFinishVelocityMode::SetVelocity,
		ActorInfo->AvatarActor->GetVelocity(),
		0.f,
		false
		);
	RootMotionTask->ReadyForActivation();
}

void UGA_Avoid::OnCancelled()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true );
}

void UGA_Avoid::OnCompleted()
{
	EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}
