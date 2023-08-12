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

bool UGetHitComponent::IsBeatenNow()
{
	if( AnimInstance )
	{
		if( HitMontage_Front && AnimInstance->Montage_IsPlaying( HitMontage_Front ) )
		{
			return true;
		}
		else if( HitMontage_Rear && AnimInstance->Montage_IsPlaying( HitMontage_Rear ) )
		{
			return true;
		}
		else if( HitMontage_Right && AnimInstance->Montage_IsPlaying( HitMontage_Right ) )
		{
			return true;
		}
		else if( HitMontage_Left && AnimInstance->Montage_IsPlaying( HitMontage_Left ) )
		{
			return true;
		}
	}

	return false;
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

	bool SameGetHitMontage = AnimInstance->Montage_IsPlaying( PlayMontage );

	AnimInstance->Montage_Play( PlayMontage );

	if( SameGetHitMontage == false )
	{
		MontageEndedDelegate.BindUObject( this, &UGetHitComponent::OnMontageEnded );
		AnimInstance->Montage_SetEndDelegate( MontageEndedDelegate, PlayMontage );
	}

	ASC->AddLooseGameplayTags( GrantingTags );
	ASC->CancelAbilities( &CancelAbilityTaskTag );
}

void UGetHitComponent::OnMontageEnded( UAnimMontage* Montage, bool bInterrupted )
{
	ASC->RemoveLooseGameplayTags( GrantingTags );
}



