// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/TagRelationship.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TagRelationship)

void UTagRelationship::GetAbilityTagsToBlockAndCancel( const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer* OutTagsToBlock, OUT FGameplayTagContainer* OutTagsToCancel ) const
{
	for( const FAbilityTagRelationship& TagRelationship : AbilityTagRelationships )
	{
		if( AbilityTags.HasTag( TagRelationship.AbilityTag ) )
		{
			if( OutTagsToBlock != nullptr )
			{
				OutTagsToBlock->AppendTags( TagRelationship.AbilityTagsToBlock );
			}

			if( OutTagsToCancel != nullptr )
			{
				OutTagsToCancel->AppendTags( TagRelationship.AbilityTagsToCancel );
			}
		}
	}
}

void UTagRelationship::GetRequiredAndBlockedActivationTags( const FGameplayTagContainer& AbilityTags, OUT FGameplayTagContainer* OutActivationRequired, OUT FGameplayTagContainer* OutActivationBlocked ) const
{
	for( const FAbilityTagRelationship& TagRelationship : AbilityTagRelationships )
	{
		if( AbilityTags.HasTag( TagRelationship.AbilityTag ) )
		{
			if( OutActivationRequired != nullptr )
			{
				OutActivationRequired->AppendTags( TagRelationship.ActivationRequiredTags );
			}

			if( OutActivationBlocked != nullptr )
			{
				OutActivationBlocked->AppendTags( TagRelationship.ActivationBlockedTags );
			}
		}
	}
}
