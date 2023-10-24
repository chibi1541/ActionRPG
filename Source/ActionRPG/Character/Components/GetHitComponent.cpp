// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/GetHitComponent.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "ARGASEnumDef.h"


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
	ASC = Character->GetARAbilitySystemComponent();
	if( !ASC )
	{
		RLOG( Error, TEXT( "GetHitComponent Initialize Failed : UARAbilitySystemComponent is Missing !! : &s" ), *GetOwner()->GetName() );
		return;
	}

	AnimInstance = Character->GetMesh()->GetAnimInstance();
	if( !AnimInstance )
	{
		RLOG( Error, TEXT( "GetHitComponent Initialize Failed : UAnimInstance is Missing !! : &s" ), *GetOwner()->GetName() );
		return;
	}

	ASC->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stiff ).AddDynamic( this, &UGetHitComponent::OnHit );
	ASC->GameplayEffectDurationChangeCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stiff ).AddDynamic( this, &UGetHitComponent::OnEffectDurationChange );
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

float UGetHitComponent::HitReaction( const FVector AttackVec )
{
	UAnimMontage* PlayMontage = GetMontagetoPlay( AttackVec );
	if( PlayMontage == nullptr )
	{
		RLOG( Error, TEXT( "Hit AnimMontage is NULL : %s" ), *GetOwner()->GetName() );
		return 0.f;
	}

	return AnimInstance->Montage_Play( PlayMontage );
}

void UGetHitComponent::OnHit( const FGameplayEffectSpec& EffectSpec )
{
	const FHitResult* HitResult = EffectSpec.GetEffectContext().GetHitResult();

	if( HitResult )
	{
		FVector HitNormal = HitResult->Normal;
		if( HitReaction( HitNormal ) != 0.f )
		{
			ASC->CancelAbilities( &CancelAbilityTaskTag );
		}
	}
}

void UGetHitComponent::OnEffectDurationChange( FActiveGameplayEffect& ActiveEffect )
{
	const FHitResult* HitResult = ActiveEffect.Spec.GetEffectContext().GetHitResult();
	if( HitResult == nullptr )
	{
		RLOG( Error, TEXT( "HitResult is NULL : %s" ), *GetOwner()->GetName() );
		return;
	}

	FVector HitNormal = HitResult->Normal;
	HitReaction( HitNormal );
}

bool UGetHitComponent::IsBackAttack( const FVector AttackVec ) const
{
	float DotProduct_Front = FVector::DotProduct( GetOwner()->GetActorRightVector(), AttackVec );

	if( DotProduct_Front > 0.f )
	{
		return true;
	}

	return false;
}

const float UGetHitComponent::GetReactionMontagePlayTime( const FVector AttackVec ) const
{
	UAnimMontage* PlayMontage = GetMontagetoPlay( AttackVec );
	if( PlayMontage == nullptr )
	{
		RLOG( Error, TEXT( "Hit AnimMontage is NULL : %s" ), *GetOwner()->GetName() );
		return 0.f;
	}

	return ( PlayMontage->GetPlayLength() / PlayMontage->RateScale );
}


