// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARPGMovementComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGMovementComponent)

UARPGMovementComponent::UARPGMovementComponent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	SprintingSpeedRate = 2.f;
	bIsSprinting = false;
}

float UARPGMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if( bIsSprinting == true )
	{
		MaxSpeed = MaxSpeed * SprintingSpeedRate;
	}

	return MaxSpeed;
}

void UARPGMovementComponent::StartSprint()
{
	bIsSprinting = true;
}

void UARPGMovementComponent::EndSprint()
{
	bIsSprinting = false;
}
