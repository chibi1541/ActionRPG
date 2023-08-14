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

	// To add mapping context
	virtual void BeginPlay();

	virtual void InitAbilitySystem() override;

	virtual void Move( const FInputActionValue& Value );
	virtual void Look( const FInputActionValue& Value );

	void Input_AbilityInputTagPressed( FGameplayTag InputTag );
	void Input_AbilityInputTagReleased( FGameplayTag InputTag );

	virtual void InitializerAttributes() override;

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

	TWeakObjectPtr<UARPGVITAttributeSet> VITAttributeSet;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
