// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA_UsedTraceHit.h"

#include "Character/BaseCharacter.h"
#include "Character/Components/HitTraceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_UsedTraceHit)

UGA_UsedTraceHit::UGA_UsedTraceHit( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_UsedTraceHit::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	ABaseCharacter* Character = Cast<ABaseCharacter>( ActorInfo->AvatarActor );
	if( Character )
	{
		if( auto HitTrace = Cast<UHitTraceComponent>( Character->GetComponentByClass( UHitTraceComponent::StaticClass() ) ) )
		{
			if( auto UsedTractHitAbility = Cast<UGA_UsedTraceHit>( Spec.GetPrimaryInstance() ) )
			{
				UsedTractHitAbility->HitTraceComp = HitTrace;

				RLOG( Warning, TEXT( "%s" ), *HitTrace->GetName());
			}
		}
	}
}


void UGA_UsedTraceHit::InitDelegateOnHit()
{
	if( HitTraceComp == nullptr )
	{
		RLOG( Error, TEXT( "HitTraceComp is null" ) );
		return;
	}

	if( HitTraceComp->OnHit.IsBound() )
	{
		HitTraceComp->OnHit.Clear();
	}

	HitTraceComp->OnHit.AddDynamic( this, &UGA_UsedTraceHit::OnHit );
}

void UGA_UsedTraceHit::OnHit_Implementation( FHitResult HitResult )
{
}

void UGA_UsedTraceHit::ResetHitTraceComp()
{
	if( HitTraceComp )
	{
		HitTraceComp->ToggleTraceCheck( false );
		HitTraceComp->OnHit.Clear();
	}
}
