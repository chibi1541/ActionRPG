// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/Attribute/ARAttributeSubsystem.h"

#include "ARGameInstance.h"
#include "Engine/DataTable.h"
#include "Subsystem/Attribute/ARDA_AttributeInfo.h"
#include "ARTableRowBase.h"
#include "Misc/EnumRange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARAttributeSubsystem)

DEFINE_ENUM_DISPLAY_NAME_TO_STRING( ECharacterType, "/Script/ActionRPG" );
DEFINE_ENUM_DISPLAY_NAME_TO_STRING( EHeroClass, "/Script/ActionRPG" );
DEFINE_ENUM_DISPLAY_NAME_TO_STRING( EMonsterType, "/Script/ActionRPG" );

ENUM_RANGE_BY_COUNT( EHeroClass, EHeroClass::Num );
ENUM_RANGE_BY_COUNT( EMonsterType, EMonsterType::Num );

void UARAttributeSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	RLOG( Warning, TEXT( "Subsystem Initialize : %s" ), *GetName() );

	const UARGameInstance* GameInstance = Cast<UARGameInstance>( GetGameInstance() );
	check( GameInstance );

	const UARDA_AttributeInfo* AttribDataAsset = GameInstance->GetAttribSubsystemDataAsset();
	check( AttribDataAsset );

	HeroCoeffInfo.Empty();
	MonsterCoeffInfo.Empty();

	// Build a Hero CoeffTable
	auto HeroCoeffTables = AttribDataAsset->GetCoefficientTables( ECharacterType::CT_Hero );
	for( const UDataTable* Table : HeroCoeffTables )
	{
		for( EHeroClass HeroClass : TEnumRange<EHeroClass>() )
		{
			const FString ClassName = EnumDisplayNameToString( HeroClass );

			FARCharCoeffTableData* Data = Table->FindRow<FARCharCoeffTableData>( FName( *ClassName ), FString(), true );
			if( Data )
			{
				RLOG( Warning, TEXT( "Build Coeff Data : %s" ), *ClassName );

				FCharacterCoefficientData Coefficient;
				Coefficient.AttackDamageCoeff = Data->AttackDamageCoeff;
				Coefficient.HealthCoeff = Data->HealthCoeff;
				Coefficient.StaminaCoeff = Data->StaminaCoeff;
				Coefficient.DefenceCoeff = Data->DefenceCoeff;
				Coefficient.ShieldGaugeCoeff = Data->ShieldGaugeCoeff;
				Coefficient.MoveSpeedCoeff = Data->MoveSpeedCoeff;
				Coefficient.AttackSpeedCoeff = Data->AttackSpeedCoeff;

				HeroCoeffInfo.Add( HeroClass, Coefficient );
			}
		}
	}

	// Build a Monster CoeffTable
	auto MonsterCoeffTables = AttribDataAsset->GetCoefficientTables( ECharacterType::CT_Monster );
	for( const UDataTable* Table : MonsterCoeffTables )
	{
		for( EMonsterType MonsterType : TEnumRange<EMonsterType>() )
		{
			const FString MonsterName = EnumDisplayNameToString( MonsterType );

			FARCharCoeffTableData* Data = Table->FindRow<FARCharCoeffTableData>( FName( *MonsterName ), FString(), true );
			if( Data )
			{
				RLOG( Warning, TEXT( "Build Coeff Data : %s" ), *MonsterName );

				FCharacterCoefficientData Coefficient;
				Coefficient.AttackDamageCoeff = Data->AttackDamageCoeff;
				Coefficient.HealthCoeff = Data->HealthCoeff;
				Coefficient.StaminaCoeff = Data->StaminaCoeff;
				Coefficient.DefenceCoeff = Data->DefenceCoeff;
				Coefficient.ShieldGaugeCoeff = Data->ShieldGaugeCoeff;
				Coefficient.MoveSpeedCoeff = Data->MoveSpeedCoeff;
				Coefficient.AttackSpeedCoeff = Data->AttackSpeedCoeff;

				MonsterCoeffInfo.Add( MonsterType, Coefficient );
			}
		}
	}
}

void UARAttributeSubsystem::Deinitialize()
{
	RLOG( Warning, TEXT( "Subsystem Deinitialize : %s" ), *GetName() );
}

const FCharacterCoefficientData* UARAttributeSubsystem::GetHeroCoeffData( EHeroClass HeroClass ) const
{
	auto CoeffiData = HeroCoeffInfo.Find( HeroClass );
	if( !CoeffiData )
	{
		RLOG( Error, TEXT( "Failed to Search Data : %s" ), *( EnumDisplayNameToString( HeroClass ) ) );
		return nullptr;
	}

	return CoeffiData;
}

const FCharacterCoefficientData* UARAttributeSubsystem::GetMonsterCoeffData( EMonsterType MonsterType ) const
{
	auto CoeffiData = MonsterCoeffInfo.Find( MonsterType );
	if( !CoeffiData )
	{
		RLOG( Error, TEXT( "Failed to Search Data : %s" ), *( EnumDisplayNameToString( MonsterType ) ) );
		return nullptr;
	}

	return CoeffiData;
}
