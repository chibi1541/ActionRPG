// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask/AT_ApplyRootMotionConstantForce.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_ApplyRootMotionConstantForce)

UAT_ApplyRootMotionConstantForce::UAT_ApplyRootMotionConstantForce( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	StrengthOverTime = nullptr;
}

UAT_ApplyRootMotionConstantForce* UAT_ApplyRootMotionConstantForce::CreateApplyRootMotionConstantForce(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	FVector WorldDirection,
	float Strength,
	float Duration,
	bool bIsAdditive,
	UCurveFloat* StrengthOverTime,
	ERootMotionFinishVelocityMode VelocityOnFinishMode,
	FVector SetVelocityOnFinish,
	float ClampVelocityOnFinish,
	bool bEnableGravity )
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration( Duration );

	UAT_ApplyRootMotionConstantForce* NewTask = NewAbilityTask<UAT_ApplyRootMotionConstantForce>( OwningAbility, TaskInstanceName );

	NewTask->WorldDirection = WorldDirection;
	NewTask->Strength = Strength;
	NewTask->Duration = Duration;
	NewTask->bIsAdditive = bIsAdditive;
	NewTask->StrengthOverTime = StrengthOverTime;
	NewTask->FinishVelocityMode = VelocityOnFinishMode;
	NewTask->FinishSetVelocity = SetVelocityOnFinish;
	NewTask->FinishClampVelocity = ClampVelocityOnFinish;
	NewTask->bEnableGravity = bEnableGravity;
	NewTask->SharedInitAndApply();

	return NewTask;
}

void UAT_ApplyRootMotionConstantForce::OnDestroy( bool bInOwnerFinished )
{
	if( MovementComponent )
	{
		MovementComponent->RemoveRootMotionSourceByID( RootMotionSourceID );
	}

	Super::OnDestroy( bInOwnerFinished );
}

void UAT_ApplyRootMotionConstantForce::TickTask( float DeltaTime )
{
	if( bIsFinished )
	{
		return;
	}

	Super::TickTask( DeltaTime );

	AActor* MyActor = GetAvatarActor();
	if( MyActor )
	{
		const bool bTimedOut = HasTimedOut();
		const bool bIsInfiniteDuration = Duration < 0.f;

		if( !bIsInfiniteDuration && bTimedOut )
		{
			// Task has finished
			bIsFinished = true;
			if( !bIsSimulating )
			{
				MyActor->ForceNetUpdate();
				if( ShouldBroadcastAbilityTaskDelegates() )
				{
					OnFinish.Broadcast();
				}
				EndTask();
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UAT_ApplyRootMotionConstantForce::SharedInitAndApply()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if( ASC && ASC->AbilityActorInfo->MovementComponent.IsValid() )
	{
		MovementComponent = Cast<UCharacterMovementComponent>( ASC->AbilityActorInfo->MovementComponent );
		StartTime = GetWorld()->GetTimeSeconds();
		EndTime = StartTime + Duration;

		if( MovementComponent )
		{
			ForceName = ForceName.IsNone() ? FName( "AT_ApplyRootMotionConstantForce" ) : ForceName;
			TSharedPtr<FRootMotionSource_ConstantForce> ConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
			ConstantForce->InstanceName = ForceName;
			ConstantForce->AccumulateMode = bIsAdditive ? ERootMotionAccumulateMode::Additive : ERootMotionAccumulateMode::Override;
			ConstantForce->Priority = 5;
			ConstantForce->Force = WorldDirection * Strength;
			ConstantForce->Duration = Duration;
			ConstantForce->StrengthOverTime = StrengthOverTime;
			ConstantForce->FinishVelocityParams.Mode = FinishVelocityMode;
			ConstantForce->FinishVelocityParams.SetVelocity = FinishSetVelocity;
			ConstantForce->FinishVelocityParams.ClampVelocity = FinishClampVelocity;
			if( bEnableGravity )
			{
				ConstantForce->Settings.SetFlag( ERootMotionSourceSettingsFlags::IgnoreZAccumulate );
			}
			RootMotionSourceID = MovementComponent->ApplyRootMotionSource( ConstantForce );

			if( Ability )
			{
				Ability->SetMovementSyncPoint( ForceName );
			}
		}
	}
	else
	{
		ABILITY_LOG( Warning, TEXT( "UAbilityTask_ApplyRootMotionConstantForce called in Ability %s with null MovementComponent; Task Instance Name %s." ),
			Ability ? *Ability->GetName() : TEXT( "NULL" ),
			*InstanceName.ToString() );
	}
}
