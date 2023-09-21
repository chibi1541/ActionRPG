// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARProjectile)

// Sets default values
AARProjectile::AARProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "PROJECTILEMOVEMENT" ) );
}

void AARProjectile::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	ProjectileMovementComp->InitialSpeed = Speed;
	ProjectileMovementComp->MaxSpeed = Speed;

	float LifeSpan = Range / Speed;

	SetLifeSpan( LifeSpan );
}
