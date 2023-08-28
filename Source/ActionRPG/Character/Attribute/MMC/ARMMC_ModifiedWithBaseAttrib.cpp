// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/MMC/ARMMC_ModifiedWithBaseAttrib.h"

#include "Character/HeroCharacter.h"
#include "Character/BaseMonster.h"
#include "Character/ARCharacterEnumDef.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARMMC_ModifiedWithBaseAttrib)

const FCharacterCoefficientData* UARMMC_ModifiedWithBaseAttrib::GetCoefficientData( const FGameplayEffectSpec& Spec ) const
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>( Spec.GetEffectContext().GetInstigator() );
	if( !BaseCharacter )
	{
		RLOG( Error, TEXT( "BaseCharacter Cast Failed" ) );
		return nullptr;
	}

	UARAttributeSubsystem* Subsystem = BaseCharacter->GetGameInstance()->GetSubsystem<UARAttributeSubsystem>();
	if( !Subsystem )
	{
		RLOG( Error, TEXT( "AttibSubsystem Cast Failed" ) );
		return nullptr;
	}

	ECharacterType CharacterType = BaseCharacter->GetCharacterType();
	if( CharacterType == ECharacterType::CT_Hero )
	{
		AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>( BaseCharacter );
		if( !HeroCharacter )
		{
			RLOG( Error, TEXT( "AHeroCharacter Cast Failed" ) );
			return nullptr;
		}

		EHeroClass HeroClass = HeroCharacter->GetHeroClass();

		return Subsystem->GetHeroCoeffData( HeroClass );
	}
	else if( CharacterType == ECharacterType::CT_Monster )
	{
		ABaseMonster* Monster = Cast<ABaseMonster>( BaseCharacter );
		if(!Monster )
		{
			RLOG( Error, TEXT( "ABaseMonster Cast Failed" ) );
			return nullptr;
		}

		EMonsterType MonsterType = Monster->GetMonsterType();

		return Subsystem->GetMonsterCoeffData( MonsterType );
	}

	return nullptr;
}
