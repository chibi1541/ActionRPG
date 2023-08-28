// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/Attribute/ARDA_AttributeInfo.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARDA_AttributeInfo)

TArray<UDataTable*> UARDA_AttributeInfo::GetCoefficientTables( ECharacterType CharacterType ) const
{
	switch( CharacterType )
	{
		case ECharacterType::CT_Hero:
		{
			return HeroCoefficientTables;
		}
		break;

		case ECharacterType::CT_Monster:
		{
			return MonsCoefficientTables;
		}
		break;

		default:
		{
			return TArray<UDataTable*>();
		}
		break;
	}
}
