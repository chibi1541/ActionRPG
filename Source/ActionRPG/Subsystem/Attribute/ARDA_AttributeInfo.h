// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARJsonFormDataAsset.h"
#include "Engine/DataTable.h"
#include "Character/ARCharacterEnumDef.h"

#include "ARDA_AttributeInfo.generated.h"


/**
 *
 */
UCLASS()
class ACTIONRPG_API UARDA_AttributeInfo : public UARJsonFormDataAsset
{
	GENERATED_BODY()

public:

	TArray<UDataTable*> GetCoefficientTables( ECharacterType CharacterType ) const;

protected:
	UPROPERTY( EditAnywhere, Category = "Coefficient|Hero" )
	TArray<TObjectPtr<UDataTable>> HeroCoefficientTables;

	UPROPERTY( EditAnywhere, Category = "Coefficient|Monster" )
	TArray<TObjectPtr<UDataTable>> MonsCoefficientTables;


};
