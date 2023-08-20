// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BaseCharacter.h"
#include "NativeGameplayTags.h"

#include "HeroCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UHeroMovementComponent;
class UInputMappingContext;
class UHeroInputConfig;
class UARPGAbilitySystemComponent;
class UTagRelationship;
class UARPGVITAttributeSet;
struct FInputActionValue;
struct FGameplayTag;


ACTIONRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN( TAG_MovingLocked );
/**
 *
 */
UCLASS()
class ACTIONRPG_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AHeroCharacter( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	
	virtual void BeginPlay() override;

	virtual void InitAbilitySystem() override;

	virtual void Move( const FInputActionValue& Value );
	virtual void Look( const FInputActionValue& Value );

	void Input_AbilityInputTagPressed( FGameplayTag InputTag );
	void Input_AbilityInputTagReleased( FGameplayTag InputTag );

	virtual void InitializerAttributes() override;

	virtual void SetHealth( float Health );
	virtual void SetStamina( float Stamina );
	virtual void SetShieldGauge( float ShieldGauge );

	virtual float GetMaxHealth() const;
	virtual float GetMaxStamina() const;
	virtual float GetMaxShieldGauge() const;

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

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Vitality" )
	TSubclassOf<UGameplayEffect> VITAttributeInitializer;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Vitality" )
	TSubclassOf<UGameplayEffect> VITRateAttributeInitializer;

	UPROPERTY()
	TObjectPtr<UARPGVITAttributeSet> VITAttributeSet;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
