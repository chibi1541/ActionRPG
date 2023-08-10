// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/GetHitComponent.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "GameplayTagContainer.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(GetHitComponent)

// Sets default values for this component's properties
UGetHitComponent::UGetHitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGetHitComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto Character = CastChecked<ABaseCharacter>( GetOwner() );
	ASC = Character->GetARPGAbilitySystemComponent();
	if( !ASC )
	{
		RLOG( Error, TEXT( "GetHitComponent Initialize Failed : UARPGAbilitySystemComponent is Missing !! : &s" ), *GetOwner()->GetName() );
		return;
	}

	AnimInstance = Character->GetMesh()->GetAnimInstance();
	if( !AnimInstance )
	{
		RLOG( Error, TEXT( "GetHitComponent Initialize Failed : UAnimInstance is Missing !! : &s" ), *GetOwner()->GetName() );
		return;
	}

}

UAnimMontage* UGetHitComponent::GetMontagetoPlay( const FVector AttackVec ) const
{
	UAnimMontage* Return = HitMontage_Front;

	float DotProduct_Right = FVector::DotProduct( GetOwner()->GetActorRightVector(), AttackVec );

	if( DotProduct_Right <= -0.5f || DotProduct_Right >= 0.5f )
	{
		// Get Hit from Right Direction, React to Left Direction
		if( DotProduct_Right > 0.f )
		{
			if( HitMontage_Left )
			{
				Return = HitMontage_Left;
			}
		}
		else
		{
			if( HitMontage_Right )
			{
				Return = HitMontage_Right;
			}
		}
	}
	else
	{
		float DotProduct_Front = FVector::DotProduct( GetOwner()->GetActorRightVector(), AttackVec );

		// GetHit from Front Direction, React to Rear Direction
		if( DotProduct_Front > 0.f )
		{
			if( HitMontage_Rear )
			{
				Return = HitMontage_Rear;
			}
		}
	}

	return Return;
}

void UGetHitComponent::HitReaction( const FVector AttackVec )
{
	UAnimMontage* PlayMontage = GetMontagetoPlay( AttackVec );
	if( PlayMontage == nullptr )
	{
		RLOG( Error, TEXT( "Hit AnimMontage is NULL : %s" ), *GetOwner()->GetName() );
		return;
	}

	AnimInstance->Montage_Play( PlayMontage );

	BlendingOutDelegate.BindUObject( this, &UGetHitComponent::OnMontageBlendingOut );
	AnimInstance->Montage_SetBlendingOutDelegate( BlendingOutDelegate, PlayMontage );

	MontageEndedDelegate.BindUObject( this, &UGetHitComponent::OnMontageEnded );
	AnimInstance->Montage_SetEndDelegate( BlendingOutDelegate, PlayMontage );

	ASC->AddLooseGameplayTags( GrantingTags );
	ASC->CancelAbilities( &CancelAbilityTaskTag );
}

void UGetHitComponent::OnMontageBlendingOut( UAnimMontage* Montage, bool bInterrupted )
{
	ASC->RemoveLooseGameplayTags( GrantingTags );

	FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage( Montage );
	if( MontageInstance )
	{
		MontageInstance->OnMontageBlendingOutStarted.Unbind();
		MontageInstance->OnMontageEnded.Unbind();
	}
}

void UGetHitComponent::OnMontageEnded( UAnimMontage* Montage, bool bInterrupted )
{
	FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage( Montage );
	if( MontageInstance )
	{
		MontageInstance->OnMontageBlendingOutStarted.Unbind();
		MontageInstance->OnMontageEnded.Unbind();
	}
}

