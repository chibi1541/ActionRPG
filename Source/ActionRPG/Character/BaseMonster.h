// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Character/BaseCharacter.h"

#include "BaseMonster.generated.h"

class UARPGAbilitySystemComponent;
class UARPGBaseAttributeSet;
class UARProtoMonsterAttributeSet;
class UARPGVITAttributeSet;
class UGameplayEffect;

/**
 *
 */

UCLASS()
class ACTIONRPG_API ABaseMonster : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseMonster( const FObjectInitializer& ObjectInitializer );

protected:	
	virtual void InitializerAttributes() override;

protected:

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HitReact|Montage" )
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TSubclassOf<UGameplayEffect> MonsterAttributeInitializer;

	UPROPERTY()
	TObjectPtr<UARProtoMonsterAttributeSet> MonsterAttributeSet;

	UPROPERTY()
	TObjectPtr<UARPGVITAttributeSet> VITBaseAttributeSet;
};
