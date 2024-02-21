// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ARVitRefAttribSet.h"

#include "Character/BaseCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Ability/ActionRPGGlobalTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARVitRefAttribSet)


UARVitRefAttribSet::UARVitRefAttribSet( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void UARVitRefAttribSet::PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue )
{

}

void UARVitRefAttribSet::PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data )
{
	Super::PostGameplayEffectExecute( Data );

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	AActor* SourceActor = nullptr;
	ABaseCharacter* SourceCharacter = nullptr;
	if( Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid() )
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceCharacter = Cast<ABaseCharacter>( SourceActor );
	}

	if( Data.EvaluatedData.Attribute == GetHealthAttribute() )
	{
		SetHealth( FMath::Clamp( GetHealth(), 0.0f, GetMaxHealth() ) );
	}
	else if( Data.EvaluatedData.Attribute == GetStaminaAttribute() )
	{
		SetStamina( FMath::Clamp( GetStamina(), 0.0f, GetMaxStamina() ) );
	}
	else if( Data.EvaluatedData.Attribute == GetShieldGaugeAttribute() )
	{
		SetShieldGauge( FMath::Clamp( GetShieldGauge(), 0.0f, GetMaxShieldGauge() ) );
	}
	else if( Data.EvaluatedData.Attribute == GetReceivedDamageAttribute() )
	{
		const float TempDamage = GetReceivedDamage();
		SetReceivedDamage( 0.f );

		if( TempDamage > 0.f )
		{
			const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();

			// Check Guard GameplayTag
			if( Data.Target.HasMatchingGameplayTag( GameplayTags.AbilityStateTag_Guard ) )
			{
				// Minus Shield Gauge
				const float NewShieldGauge = GetShieldGauge() - TempDamage;
				SetShieldGauge( FMath::Clamp( NewShieldGauge, 0.f, GetMaxShieldGauge() ) );
				if( NewShieldGauge <= 0.f )
				{
					// Remove Guard Ability
					FGameplayTagContainer TagContainer = FGameplayTagContainer( GameplayTags.AbilityActionTag_Guard );
					Data.Target.CancelAbilities( &TagContainer );

					// Minus health Value
					const float NewHealth = GetHealth() + NewShieldGauge;
					SetHealth( FMath::Clamp( NewHealth, 0.f, GetMaxHealth() ) );
				}
			}
			else
			{
				// Minus health Value
				const float NewHealth = GetHealth() - TempDamage;
				SetHealth( FMath::Clamp( NewHealth, 0.f, GetMaxHealth() ) );
			}
		}
	}
}

void UARVitRefAttribSet::AdjustAttributeForMaxChange( FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute,
	float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty )
{

}
