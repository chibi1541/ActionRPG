// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGA_ComboAttack_Moving.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGA_ComboAttack_Moving)

UARGA_ComboAttack_Moving::UARGA_ComboAttack_Moving( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void UARGA_ComboAttack_Moving::OnPlayMontage( int CurrentIndex )
{
	Super::OnPlayMontage( CurrentIndex );

	if( CurComboIndex >= MoveDistance.Num() || CurComboIndex >= MoveDuration.Num() )
	{
		// Error Log
		return;
	}

	auto Character = GetAvatarActorFromActorInfo();
	FVector FrontVec = Character->GetActorForwardVector();

	float Strength = MoveDistance[CurComboIndex] / MoveDuration[CurComboIndex];

	static auto ConstantForceTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		NAME_None,
		FrontVec,
		Strength,
		MoveDuration[CurComboIndex],
		false, 
		nullptr,
		ERootMotionFinishVelocityMode::SetVelocity,
		FVector(0.f, 0.f, 0.f),
		0.f,
		true
		);

		
	if( ConstantForceTask->IsFinished() )
	{
		RLOG(Warning, TEXT("Test"));
	}

	ConstantForceTask->ReadyForActivation();


}
