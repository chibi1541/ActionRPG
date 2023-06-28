// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARPGGameplayAbility.h"

#include "AbilitySystemGlobals.h"
#include "Ability/ARPGAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGGameplayAbility)

bool UARPGGameplayAbility::DoesAbilitySatisfyTagRequirements( const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags ) const
{
	bool bBlocked = false;
	bool bMissing = false;

	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	// Check if any of this ability's tags are currently blocked
	if( AbilitySystemComponent.AreAbilityTagsBlocked( AbilityTags ) )
	{
		bBlocked = true;
	}

	const UARPGAbilitySystemComponent* ArpgASC = Cast<UARPGAbilitySystemComponent>( &AbilitySystemComponent );
	FGameplayTagContainer ModiActivationBlockedTags = ActivationBlockedTags;
	FGameplayTagContainer ModiActivationRequiredTags = ActivationRequiredTags;

	if( ArpgASC != nullptr )
	{
		ArpgASC->GetAdditionalActivationTagRequirements( AbilityTags, ModiActivationRequiredTags, ModiActivationBlockedTags );
	}

	// Check to see the required/blocked tags for this ability
	if( ModiActivationRequiredTags.Num() || ModiActivationBlockedTags.Num() )
	{
		static FGameplayTagContainer AbilitySystemComponentTags;
		AbilitySystemComponentTags.Reset();

		AbilitySystemComponent.GetOwnedGameplayTags( AbilitySystemComponentTags );

		if( AbilitySystemComponentTags.HasAny( ModiActivationBlockedTags ) )
		{
			bBlocked = true;
		}

		if( !AbilitySystemComponentTags.HasAll( ModiActivationRequiredTags ) )
		{
			bMissing = true;
		}
	}

	if( SourceTags != nullptr )
	{
		if( SourceBlockedTags.Num() || SourceRequiredTags.Num() )
		{
			if( SourceTags->HasAny( SourceBlockedTags ) )
			{
				bBlocked = true;
			}

			if( !SourceTags->HasAll( SourceRequiredTags ) )
			{
				bMissing = true;
			}
		}
	}

	if( TargetTags != nullptr )
	{
		if( TargetBlockedTags.Num() || TargetRequiredTags.Num() )
		{
			if( TargetTags->HasAny( TargetBlockedTags ) )
			{
				bBlocked = true;
			}

			if( !TargetTags->HasAll( TargetRequiredTags ) )
			{
				bMissing = true;
			}
		}
	}

	if( bBlocked )
	{
		if( OptionalRelevantTags && BlockedTag.IsValid() )
		{
			OptionalRelevantTags->AddTag( BlockedTag );
		}
		return false;
	}
	if( bMissing )
	{
		if( OptionalRelevantTags && MissingTag.IsValid() )
		{
			OptionalRelevantTags->AddTag( MissingTag );
		}
		return false;
	}

	return true;
}
