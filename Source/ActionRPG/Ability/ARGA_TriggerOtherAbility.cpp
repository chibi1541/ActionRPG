// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_TriggerOtherAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_TriggerOtherAbility)


void UARGA_TriggerOtherAbility::MakePayloadData_Implementation( FGameplayEventData& Payload )
{
	Payload = FGameplayEventData();
}

void UARGA_TriggerOtherAbility::TriggerAbility()
{
	FGameplayEventData Payload;
	MakePayloadData( Payload );

	SendGameplayEvent( TriggerAbilityTag, Payload );
}
