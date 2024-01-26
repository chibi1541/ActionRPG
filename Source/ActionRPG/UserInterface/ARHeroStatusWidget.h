// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "ARHeroStatusWidget.generated.h"

class UARCharacterStateComponent;
class UTextBlock;
class UWidgetSwitcher;
class UProgressBar;


UCLASS( Blueprintable )
class ACTIONRPG_API UARHeroStatusWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UARHeroStatusWidget( const FObjectInitializer& ObjectInitializer );


	UFUNCTION( BlueprintCallable )
		void InitializeStatusWidget( UARCharacterStateComponent* StateComponent );

#pragma region OnAtrributeChanged

	UFUNCTION()
		void OnChangedCurrentHealthPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedMaxHealthPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedCurrentManaPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedMaxManaPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedCurrentStaminaPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedMaxStaminaPoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedCurrentShieldGaugePoint( float OldValue, float NewValue );

	UFUNCTION()
		void OnChangedMaxShieldGaugePoint( float OldValue, float NewValue );


	FORCEINLINE void ChangeHealthPointRate();

	FORCEINLINE void ChangeManaPointRate();

	FORCEINLINE void ChangeStaminaPointRate();

	FORCEINLINE void ChangeShieldGaugePointRate();

#pragma endregion



	void UpdateHPText();

	void UpdateManaText();

	void UpdateShieldGaugeText();

	void UpdateStaminaText();


protected:
	virtual void NativeOnListItemObjectSet( UObject* ListItemObject ) override;



protected:

	// Health Point

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float CurHealthPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float MaxHealthPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float HealthPointRate;

	// Mana Point

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float CurManaPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float MaxManaPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float ManaPointRate;

	// Stamina Point

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float CurStaminaPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float MaxStaminaPoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float StaminaPointRate;

	// ShieldGauge Point

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float CurShieldGaugePoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float MaxShieldGaugePoint;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, meta = ( ClampMin = "0.0" ) )
		float ShieldGaugePointRate;

protected:

#pragma region UI

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UWidgetSwitcher> EmblemSwitcher;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UTextBlock> LevelText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UTextBlock> HPText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UTextBlock> ManaText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UTextBlock> ShieldGaugeText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UTextBlock> StaminaText;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UProgressBar> HPBar;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UProgressBar> ManaBar;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UProgressBar> ShieldGaugeBar;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
		TObjectPtr<UProgressBar> StaminaBar;

#pragma endregion

private:

	TWeakObjectPtr<UARCharacterStateComponent> HeroStateComponent;

};
