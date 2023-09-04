// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARMovementComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMovementComponent)

UARMovementComponent::UARMovementComponent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	SprintingSpeedRate = 2.f;
	bIsSprinting = false;
}

float UARMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if( bIsSprinting == true )
	{
		MaxSpeed = MaxSpeed * SprintingSpeedRate;
	}

	return MaxSpeed;
}

void UARMovementComponent::StartSprint()
{
	bIsSprinting = true;
}

void UARMovementComponent::EndSprint()
{
	bIsSprinting = false;
}
