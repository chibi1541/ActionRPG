// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/HeroInputComponent.h"
#include "InputActionValue.h"
#include "GameplayTagsManager.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroCharacter)

AHeroCharacter::AHeroCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer )
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize( 42.f, 96.0f );

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, 500.0f, 0.0f ); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( RootComponent );
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

}

void AHeroCharacter::SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	RCHECK( DefaultMappingContext );
	RCHECK( InputConfig );

	//Add Input Mapping Context
	if( APlayerController* PlayerController = Cast<APlayerController>( GetController() ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer() ) )
		{
			Subsystem->AddMappingContext( DefaultMappingContext, 0 );
		}
	}

	UHeroInputComponent* HeroIC = CastChecked<UHeroInputComponent>( PlayerInputComponent );

	//HeroIC->BindAbilityActions(InputConfig, )

	//HeroIC->BindNativeAction( InputConfig, InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move, true );
	//HeroIC->BindNativeAction( InputConfig, InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look, true );
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroCharacter::Move( const FInputActionValue& Value )
{
	FVector2D MovementVector = Value.Get<FVector2D>();

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
		AddMovementInput( ForwardDirection, MovementVector.Y );
		AddMovementInput( RightDirection, MovementVector.X );
	}
}

void AHeroCharacter::Look( const FInputActionValue& Value )
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if( Controller != nullptr )
	{
		// add yaw and pitch input to controller
		AddControllerYawInput( LookAxisVector.X );
		AddControllerPitchInput( LookAxisVector.Y );
	}
}

//void AHeroCharacter::Input_AbilityInputTagPressed( FGameplayTag InputTag )
//{
//	
//}
//
//void AHeroCharacter::Input_AbilityInputTagReleased( FGameplayTag InputTag )
//{
//
//}
