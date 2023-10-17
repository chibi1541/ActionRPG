// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ARUsedTraceHitInterface.h"

#include "Character/Components/HitTraceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARUsedTraceHitInterface)

void IARUsedTraceHitInterface::InitHitTraceOnHit_Implementation( UHitTraceComponent* Component )
{
	if( Component )
	{
		HitTraceComp = Component;
	}
}

void IARUsedTraceHitInterface::InitDelegateOnHit_Implementation()
{
	if( !HitTraceComp.IsValid() )
	{
		RLOG( Error, TEXT( "HitTraceComp is null" ) );
		return;
	}

	if( HitTraceComp->OnHit.IsBound() )
	{
		HitTraceComp->OnHit.Clear();
	}

	HitTraceComp->OnHit.AddDynamic( this, &IARUsedTraceHitInterface::OnHit );
}

void IARUsedTraceHitInterface::ResetHitTraceComp_Implementation()
{
	if( HitTraceComp.IsValid() )
	{
		HitTraceComp->ToggleTraceCheck( false );
		HitTraceComp->OnHit.Clear();
	}
}
