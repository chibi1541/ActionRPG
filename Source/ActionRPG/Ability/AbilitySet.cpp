// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilitySet.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilitySet)

UAbilitySet::UAbilitySet( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{

}

void UAbilitySet::GiveToAbilitySystem( UAbilitySystemComponent* AbilitySystemComponent ) const
{
	RCHECK( AbilitySystemComponent != nullptr );

	for( int32 AbilityIndex = 0; AbilityIndex < Abilities.Num(); AbilityIndex++ )
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = Abilities[AbilityIndex];

		if( IsValid( AbilityToGrant.Ability ) == false )
		{
			RLOG( Error, TEXT( "GrantedGameplayAbilities[%d] on ability set [%s] is not valid." ), AbilityIndex, *GetNameSafe( this ) )
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec( AbilityCDO, 1 );
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
