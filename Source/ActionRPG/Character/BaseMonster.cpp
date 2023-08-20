// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseMonster.h"

#include "Character/BaseAIController.h"
#include "Ability/ARPGAbilitySystemComponent.h"
#include "Character/Attribute/ARPGBaseAttributeSet.h"
#include "Character/Attribute/ARProtoMonsterAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseMonster)

ABaseMonster::ABaseMonster( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	AIControllerClass = ABaseAIController::StaticClass();

	MonsterAttributeSet = CreateDefaultSubobject<UARProtoMonsterAttributeSet>( "ARProtoMonsterAttributeSet" );
}

void ABaseMonster::InitializerAttributes()
{
	Super::InitializerAttributes();

	if( !MonsterAttributeInitializer )
	{
		RLOG( Error, TEXT( "MonsterAttributeInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec( MonsterAttributeInitializer, GetCharacterLevel(), EffectContext );
	if( NewHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *NewHandle.Data.Get(), AbilitySystemComponent.Get() );
	}
}
