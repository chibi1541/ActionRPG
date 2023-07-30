// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask/AT_WaitInputRelease.h"
#include "Ability/ARPGGameplayAbility.h"
#include "Engine/World.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(AT_WaitInputRelease)

UAT_WaitInputRelease::UAT_WaitInputRelease( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	StartTime = 0.0f;
}

UAT_WaitInputRelease* UAT_WaitInputRelease::CreateWaitInputRelease( UGameplayAbility* OwningAbility )
{
	UAT_WaitInputRelease* NewTask = NewAbilityTask<UAT_WaitInputRelease>(OwningAbility);
	return NewTask;
}

void UAT_WaitInputRelease::OnReleaseCallBack()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;

	UARPGGameplayAbility* ARPGAbility = Cast<UARPGGameplayAbility>(Ability.Get());
	if(ARPGAbility == nullptr)
	{
		return;
	}

	ARPGAbility->OnReleased.Remove( DelegateHandle );

	OnRelease.Broadcast( ElapsedTime );

	EndTask();
}

void UAT_WaitInputRelease::Activate()
{
	StartTime = GetWorld()->GetTimeSeconds();

	UARPGGameplayAbility* ARPGAbility = Cast<UARPGGameplayAbility>( Ability.Get() );
	if( ARPGAbility == nullptr )
	{
		EndTask();
		return;
	}

	DelegateHandle = ARPGAbility->OnReleased.AddUObject(this, &UAT_WaitInputRelease::OnReleaseCallBack );
}
