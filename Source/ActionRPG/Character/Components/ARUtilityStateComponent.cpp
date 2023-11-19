// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARUtilityStateComponent.h"

// Sets default values for this component's properties
UARUtilityStateComponent::UARUtilityStateComponent( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UARUtilityStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

