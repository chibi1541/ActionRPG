// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Blueprint/UserWidget.h"

#include "ARMonsterHPBarWidget.generated.h"

/**
 *
 */
class UARCharacterStateComponent;


UCLASS()
class ACTIONRPG_API UARMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
		void SetHPRate( float HPRate );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
		void SetManaRate( float ManaRate );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
		void SetMonsterName( const FText& MonsterName );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
		void SetCurrentHP( float CurrentHP );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable )
		void SetMaxHP( float MaxHP );

	UFUNCTION( BlueprintCallable )
		void InitializeWidget( UARCharacterStateComponent* StateComponent );



protected:
	UFUNCTION()
		void OnChangedCurrentHealthPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedMaxHealthPoint( float OldValue, float NewValue );


protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float CurHealthPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float MaxHealthPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float HealthPointRate;
};
