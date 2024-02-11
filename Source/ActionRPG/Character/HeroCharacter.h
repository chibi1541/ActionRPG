// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BaseCharacter.h"
#include "NativeGameplayTags.h"
#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "HeroCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UHeroMovementComponent;
class UARComboAttackComponent;
class UInputMappingContext;
class UHeroInputConfig;
class UTagRelationship;
class UARVitRefAttribSet;
class UARAttackAttribSet;
class UARAgiRefAttribSet;
class UARIntRefAttribSet;
struct FInputActionValue;
struct FGameplayTag;

class UUserWidget;


ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN( TAG_MovingLocked );
/**
 *
 */
UCLASS()
class ACTIONRPG_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AHeroCharacter( const FObjectInitializer& ObjectInitializer );

#pragma region Get_Attribute

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetMaxHealth() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetCurrentHealth() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|IntAttributes" )
		virtual float GetMaxMana() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|IntAttributes" )
		virtual float GetCurrentMana() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetMaxStamina() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetCurrentStamina() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetMaxShieldGauge() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|HealthAttributes" )
		virtual float GetCurrentShieldGauge() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|AgilityAttributes" )
		virtual float GetMoveSpeed() const;

	UFUNCTION( BlueprintCallable, Category = "ActionRPG|AgilityAttributes" )
		virtual float GetAttackSpeed() const;

#pragma endregion

	UFUNCTION( BlueprintCallable, Category = "Input|InputAction" )
		virtual const UInputAction* GetInputAction( const FGameplayTag InputTag ) const;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	virtual void PossessedBy( AController* NewController ) override;

	virtual void UnPossessed() override;

	virtual void BeginPlay() override;

	virtual void InitAbilitySystem() override;

	virtual void Tick( float DeltaSeconds ) override;

	virtual void Move( const FInputActionValue& Value );
	virtual void Look( const FInputActionValue& Value );
	virtual void Confirm();
	virtual void ComboAttack();

	virtual void BeginDestroy() override;

	void Input_AbilityInputTagPressed( FGameplayTag InputTag );
	void Input_AbilityInputTagReleased( FGameplayTag InputTag );

	virtual void SetHealth( float Health );
	virtual void SetMana( float Mana );
	virtual void SetStamina( float Stamina );
	virtual void SetShieldGauge( float ShieldGauge );

	virtual void Die() override;

private:
	void SetHeroStatusWidget( const AController* NewController );

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
		TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
		TObjectPtr<UCameraComponent> FollowCamera;


	//Input
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input )
		TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input )
		TObjectPtr<UHeroInputConfig> InputConfig;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Ability )
		TObjectPtr<UTagRelationship> TagRelationshipTable;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Health" )
		TSubclassOf<UGameplayEffect> HealthAttribInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Attack" )
		TSubclassOf<UGameplayEffect> AttackAttribInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Agility" )
		TSubclassOf<UGameplayEffect> AgilityRefAttribInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute|Intelligence" )
		TSubclassOf<UGameplayEffect> IntelligenceRefAttribInitializer;

	UPROPERTY()
		TObjectPtr<UARVitRefAttribSet> VitRefAttribSet;

	UPROPERTY()
		TObjectPtr<UARAttackAttribSet> AttackAttribSet;

	UPROPERTY()
		TObjectPtr<UARAgiRefAttribSet> AgiRefAttribSet;

	UPROPERTY()
		TObjectPtr<UARIntRefAttribSet> IntRefAttribSet;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
		TObjectPtr<UARComboAttackComponent> ComboAttackComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "ClassType" )
		EHeroClass HeroClass;

#pragma region HUD_WIDGET

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Status" )
		TSubclassOf<UUserWidget> StatusWidget;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Status" )
		FGameplayTag MainStatusWidgetTag;

	FUIExtensionHandle MainStatusWidgetHandle;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "HUD|Status" )
		FGameplayTag SubStatusChannel;


#pragma endregion

private:
	bool bInitialize;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UARComboAttackComponent* GetComboAttackComponent() const { return ComboAttackComponent; }

	FORCEINLINE EHeroClass GetHeroClass() const { return HeroClass; }
};
