// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARTargetComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARTargetComponent)

// Sets default values for this component's properties
UARTargetComponent::UARTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

const FVector UARTargetComponent::GetTargetLocation() const
{
	return (GetOwner()) ? GetOwner()->GetActorLocation() + GetComponentLocation() : FVector::Zero();
}

