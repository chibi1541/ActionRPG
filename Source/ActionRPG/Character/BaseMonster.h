// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Character/BaseCharacter.h"

#include "BaseMonster.generated.h"

class UARAttackAttribSet;
class UARVitRefAttribSet;
class UARAgiRefAttribSet;
class UGameplayEffect;
class UARMonsterHPBarWidget;

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
	virtual void BeginPlay() override;

	virtual void SetHealth( float Health );
	virtual void SetStamina( float Stamina );
	virtual void SetShieldGauge( float ShieldGauge );


	virtual void FinishDying() override;

	virtual void Die() override;

public:
	virtual float GetCurrentHealth() const;
	virtual float GetCurrentStamina() const;

	virtual float GetMaxHealth() const;
	virtual float GetMaxStamina() const;
	virtual float GetMaxShieldGauge() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|AgilityAttributes" )
	virtual float GetMoveSpeed() const;

protected:

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "UI|HPBar" )
		TSubclassOf<class UARMonsterHPBarWidget> FloatingHPBarClass;

	UPROPERTY()
		class UARMonsterHPBarWidget* FloatingHPBar;

	UPROPERTY( BlueprintReadOnly, VisibleAnywhere, Category = "UI|HPBar" )
		class UWidgetComponent* FloatingHPBarComponent;


	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Montage|Dying" )
		TArray<TObjectPtr<UAnimMontage>> DyingMontages;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Health" )
		TSubclassOf<UGameplayEffect> HealthAttribInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute|Attack" )
		TSubclassOf<UGameplayEffect> AttackAttribInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Agility" )
		TSubclassOf<UGameplayEffect> AgilityRefAttribInitializer;

	UPROPERTY()
		TObjectPtr<UARAttackAttribSet> AttackAttribSet;

	UPROPERTY()
		TObjectPtr<UARVitRefAttribSet> VitRefAttribSet;

	UPROPERTY()
		TObjectPtr<UARAgiRefAttribSet> AgiRefAttribSet;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "MonsterType" )
		EMonsterType MonsterType;

public:
	FORCEINLINE EMonsterType GetMonsterType() const { return MonsterType; }

};
