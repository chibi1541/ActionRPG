// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ARIntRefAttribSet.h"

#include "Character/BaseCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARIntRefAttribSet)

UARIntRefAttribSet::UARIntRefAttribSet( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void UARIntRefAttribSet::PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue )
{

}

void UARIntRefAttribSet::PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data )
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


	if( Data.EvaluatedData.Attribute == GetManaAttribute() )
	{
		SetMana( FMath::Clamp( GetMana(), 0.0f, GetMaxMana() ) );
	}
}
