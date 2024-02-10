// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameState)


AARGameState::AARGameState( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	MinionKillCount = 0;
}


void AARGameState::MinionKillCount()
{
	MinionKillCount++;
}

const int AARGameState::GetMinionKillCount() const
{
	return MinionKillCount;
}