// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BaseCharacter.h"

#include "HeroCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UHeroMovementComponent;
class UInputMappingContext;
class UHeroInputConfig;
struct FInputActionValue;

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

	virtual void Move( const FInputActionValue& Value );
	virtual void Look( const FInputActionValue& Value );

	//void Input_AbilityInputTagPressed( FGameplayTag InputTag );
	//void Input_AbilityInputTagReleased( FGameplayTag InputTag );

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UCameraComponent> FollowCamera;

	//UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Components, meta = ( AllowPrivateAccess = "true" ) )
	//TObjectPtr<UHeroMovementComponent> HeroMovementComp;

	//Input
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UHeroInputConfig> InputConfig;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
