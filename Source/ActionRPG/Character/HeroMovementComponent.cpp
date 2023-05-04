// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroMovementComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroMovementComponent)

// Sets default values for this component's properties
UHeroMovementComponent::UHeroMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHeroMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto Pawn = GetPawn<APawn>();
	auto Controller = Pawn ? GetController<AController>() : nullptr;

	//Add Input Mapping Context
	if( APlayerController* PlayerController = Cast<APlayerController>( Controller ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ) )
		{

			Subsystem->AddMappingContext( DefaultMappingContext, 0 );
		}
	}

	// Set up action bindings
	if( UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( Pawn->InputComponent ) )
	{
		EnhancedInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &UHeroMovementComponent::Move );

		EnhancedInputComponent->BindAction( LookAction, ETriggerEvent::Triggered, this, &UHeroMovementComponent::Look );
	}
}

void UHeroMovementComponent::Move( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	auto Pawn = GetPawn<APawn>();
	auto Controller = Pawn ? GetController<AController>() : nullptr;

	if( Controller != nullptr )
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

		// get right vector 
		const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		// add movement 
		Pawn->AddMovementInput( ForwardDirection, MovementVector.Y );
		Pawn->AddMovementInput( RightDirection, MovementVector.X );
	}
}

void UHeroMovementComponent::Look( const FInputActionValue& Value )
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	auto Pawn = GetPawn<APawn>();
	auto Controller = Pawn ? GetController<AController>() : nullptr;

	if( Controller != nullptr )
	{
		// add yaw and pitch input to controller
		Pawn->AddControllerYawInput( LookAxisVector.X );
		Pawn->AddControllerPitchInput( LookAxisVector.Y );
	}
}

