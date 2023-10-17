// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilitySet.h"

#include "AbilitySystemComponent.h"
#include "Character/HeroCharacter.h"
#include "Character/Components/ARComboAttackComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilitySet)

UAbilitySet::UAbilitySet( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{

}

void UAbilitySet::GiveToAbilitySystem( UAbilitySystemComponent* AbilitySystemComponent ) const
{
	RCHECK( AbilitySystemComponent != nullptr );

	for( int32 AbilityIndex = 0; AbilityIndex < KeyBindingAbilities.Num(); AbilityIndex++ )
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = KeyBindingAbilities[AbilityIndex];

		if( IsValid( AbilityToGrant.Ability ) == false )
		{
			RLOG( Error, TEXT( "KeyBindingAbilities[%d] on ability set [%s] is not valid." ), AbilityIndex, *GetNameSafe( this ) )
				continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec( AbilityCDO, 1 );
		AbilitySpec.DynamicAbilityTags.AddTag( AbilityToGrant.InputTag );

		AbilitySystemComponent->GiveAbility( AbilitySpec );
	}

	for( int32 AbilityIndex = 0; AbilityIndex < Abilities.Num(); AbilityIndex++ )
	{
		auto Ability = Abilities[AbilityIndex];

		if( IsValid( Ability ) == false )
		{
			RLOG( Error, TEXT( "Abilities[%d] on ability set [%s] is not valid." ), AbilityIndex, *GetNameSafe( this ) )
				continue;
		}

		UGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec( AbilityCDO, 1 );

		AbilitySystemComponent->GiveAbility( AbilitySpec );
	}

	auto Hero = Cast<AHeroCharacter>( AbilitySystemComponent->GetAvatarActor() );
	if( Hero )
	{
		for( int32 AbilityIndex = 0; AbilityIndex < ComboAttackAbilities.Num(); AbilityIndex++ )
		{
			auto Ability = ComboAttackAbilities[AbilityIndex];

			if( IsValid( Ability ) == false )
			{
				RLOG( Error, TEXT( "ComboAttackAbilities[%d] on ability set [%s] is not valid." ), AbilityIndex, *GetNameSafe( this ) )
					continue;
			}

			UGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UGameplayAbility>();
			FGameplayAbilitySpec AbilitySpec( AbilityCDO, 1 );

			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility( AbilitySpec );

			if( Handle.IsValid() )
			{
				Hero->GetComboAttackComponent()->AddAttackAbilityHandle( Handle );
			}
		}
	}
}
