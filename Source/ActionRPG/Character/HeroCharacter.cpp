// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"

#include "Ability/ActionRPGGlobalTags.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/HeroInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/Components/ARCharacterStateComponent.h"
#include "Character/Components/ARTargetingComponent.h"
#include "Character/Components/ARComboAttackComponent.h"
#include "Camera/PlayerCameraManager.h"

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARAgiRefAttribSet.h"
#include "Character/Attribute/ARIntRefAttribSet.h"

#include "Blueprint/UserWidget.h"
#include "Subsystem/GameplayMessage/ARGameplayMessageSubsystem.h"
#include "UserInterface/SubHeroStatusMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroCharacter)

UE_DEFINE_GAMEPLAY_TAG( TAG_MovingLocked, "Gameplay.MovingLocked" );

AHeroCharacter::AHeroCharacter( const FObjectInitializer& ObjectInitializer )
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
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( RootComponent );
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	VitRefAttribSet = CreateDefaultSubobject<UARVitRefAttribSet>( TEXT( "ARVitRefAttribSet" ) );

	AttackAttribSet = CreateDefaultSubobject<UARAttackAttribSet>( TEXT( "ARAttackAttribSet" ) );

	AgiRefAttribSet = CreateDefaultSubobject<UARAgiRefAttribSet>( TEXT( "ARAgiRefAttribSet" ) );

	IntRefAttribSet = CreateDefaultSubobject<UARIntRefAttribSet>( TEXT( "ARIntRefAttribSet" ) );

	// Create a ComboAttackComponent
	ComboAttackComponent = CreateDefaultSubobject<UARComboAttackComponent>( TEXT( "COMBOATTACKCOMP" ) );

	bInitialize = false;
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

	const FActionRPGGlobalTags& GameplayTags = FActionRPGGlobalTags::Get();

	HeroIC->BindAbilityActions( InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased );

	HeroIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move, true );
	HeroIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look, true );
	HeroIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Confirm, ETriggerEvent::Triggered, this, &ThisClass::Confirm, true );
	HeroIC->BindNativeAction( InputConfig, GameplayTags.InputTag_Attack, ETriggerEvent::Triggered, this, &ThisClass::ComboAttack, true );
}

void AHeroCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	if( bInitialize )
	{
		SetHeroStatusWidget( NewController );
	}
}


void AHeroCharacter::UnPossessed()
{
	if( MainStatusWidgetHandle.IsValid() )
		MainStatusWidgetHandle.Unregister();

	Super::UnPossessed();
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetHealth( GetMaxHealth() );
	SetStamina( GetMaxStamina() );
	SetShieldGauge( GetMaxShieldGauge() );
	SetMana( GetMaxMana() );

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if( AgiRefAttribSet )
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate( AgiRefAttribSet->GetModifiedMoveSpeedAttribute() ).AddUObject( this, &AHeroCharacter::OnSpeedChange );
	}

	if( auto NewController = GetController() )
	{
		SetHeroStatusWidget( NewController );
	}

	bInitialize = true;
}

void AHeroCharacter::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	if( TagRelationshipTable != nullptr )
	{
		AbilitySystemComp->SetTagRelationshipTable( TagRelationshipTable );
	}
}

void AHeroCharacter::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	if( GetWorld()->IsPaused() )
	{
		APlayerController* PlayerController = Cast<APlayerController>( GetController() );
		if( PlayerController )
		{
			GetWorld()->bIsCameraMoveableWhenPaused = true;
			PlayerController->UpdateCameraManager( DeltaSeconds );
		}
	}
}

void AHeroCharacter::Move( const FInputActionValue& Value )
{
	if( AbilitySystemComp->HasMatchingGameplayTag( TAG_MovingLocked ) == true || GetDeadState() )
		return;

	if( GetWorld()->IsPaused() )
	{
		return;
	}

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
	if( TargetingComponent->GetTargetCharacter() )
	{
		return;
	}

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if( Controller != nullptr )
	{
		// add yaw and pitch input to controller
		AddControllerYawInput( LookAxisVector.X );
		AddControllerPitchInput( LookAxisVector.Y );
	}
}

void AHeroCharacter::Confirm()
{
	if( AbilitySystemComp )
	{
		AbilitySystemComp->InputConfirm();
	}
}

void AHeroCharacter::ComboAttack()
{
	if( ComboAttackComponent )
	{
		ComboAttackComponent->StartComboAttack();
	}
}

void AHeroCharacter::BeginDestroy()
{
	if( MainStatusWidgetHandle.IsValid() )
		MainStatusWidgetHandle.Unregister();

	Super::BeginDestroy();
}

void AHeroCharacter::Input_AbilityInputTagPressed( FGameplayTag InputTag )
{
	if( GetWorld()->IsPaused() && !InputConfig->IsTriggerWhenPaused( InputTag, false ) )
	{
		return;
	}

	if( UARAbilitySystemComponent* ArASC = GetARAbilitySystemComponent() )
	{
		ArASC->AbilityInputTagPressed( InputTag );
	}
}

void AHeroCharacter::Input_AbilityInputTagReleased( FGameplayTag InputTag )
{
	if( GetWorld()->IsPaused() && !InputConfig->IsTriggerWhenPaused( InputTag, false ) )
	{
		return;
	}

	if( UARAbilitySystemComponent* ArASC = GetARAbilitySystemComponent() )
	{
		ArASC->AbilityInputTagReleased( InputTag );
	}
}

void AHeroCharacter::SetHealth( float Health )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetHealth( Health );
	}
}

void AHeroCharacter::SetMana( float Mana )
{
	if( IntRefAttribSet )
	{
		IntRefAttribSet->SetMana( Mana );
	}
}

void AHeroCharacter::SetStamina( float Stamina )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetStamina( Stamina );
	}
}

void AHeroCharacter::SetShieldGauge( float ShieldGauge )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetShieldGauge( ShieldGauge );
	}
}

void AHeroCharacter::Die()
{
	Super::Die();

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	GetMesh()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector( 0 );
}

void AHeroCharacter::SetHeroStatusWidget( const AController* NewController )
{
	if( NewController->IsPlayerController() )
	{
		auto PlayerController = Cast<APlayerController>( NewController );
		PlayerController->PlayerCameraManager->ViewPitchMin = -60.f;
		PlayerController->PlayerCameraManager->ViewPitchMax = 40.f;

		// Create Main HUD Widget
		if( UARGameUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UARGameUIManagerSubsystem>() )
		{
			MainStatusWidgetHandle = UIManager->RegisterExtensionWidget( MainStatusWidgetTag, StatusWidget, GetCharacterStateComponenet() );
		}
	}
	else
	{
		if( UARGameplayMessageSubsystem* MessageManager = GetGameInstance()->GetSubsystem<UARGameplayMessageSubsystem>() )
		{
			FSubHeroStatusMessage Message;
			Message.StateComponent = GetCharacterStateComponenet();

			MessageManager->BroadcastMessage( SubStatusChannel, Message );
		}
	}
}

float AHeroCharacter::GetMaxHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxHealth();
	}

	return 0.f;
}

float AHeroCharacter::GetCurrentHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetHealth();
	}

	return 0.f;
}

float AHeroCharacter::GetMaxMana() const
{
	if( IntRefAttribSet )
	{
		return IntRefAttribSet->GetMaxMana();
	}

	return 0.f;
}

float AHeroCharacter::GetCurrentMana() const
{
	if( IntRefAttribSet )
	{
		return IntRefAttribSet->GetMana();
	}

	return 0.f;
}

float AHeroCharacter::GetMaxStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxStamina();
	}

	return 0.f;
}

float AHeroCharacter::GetCurrentStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetStamina();
	}

	return 0.f;
}

float AHeroCharacter::GetMaxShieldGauge() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxShieldGauge();
	}

	return 0.f;
}

float AHeroCharacter::GetCurrentShieldGauge() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetShieldGauge();
	}

	return 0.f;
}

float AHeroCharacter::GetMoveSpeed() const
{
	if( AgiRefAttribSet )
	{
		return AgiRefAttribSet->GetModifiedMoveSpeed();
	}

	return 0.f;
}

float AHeroCharacter::GetAttackSpeed() const
{
	if( AgiRefAttribSet )
	{
		return AgiRefAttribSet->GetAttackSpeed();
	}

	return 0.f;
}

const UInputAction* AHeroCharacter::GetInputAction( const FGameplayTag InputTag ) const
{
	if( !InputConfig )
	{
		return nullptr;
	}

	return InputConfig->FindAbilityInputActionForTag( InputTag, true );
}
