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

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARAgiRefAttribSet.h"
#include "Character/Attribute/ARIntRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HeroCharacter)

UE_DEFINE_GAMEPLAY_TAG( TAG_MovingLocked, "Gameplay.MovingLocked" );

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

	if( !PassiveEffects.IsEmpty() )
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();

		for( auto Effect : PassiveEffects )
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComp->MakeOutgoingSpec( Effect, GetCharacterLevel(), EffectContext );
			if( NewHandle.IsValid() )
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *NewHandle.Data.Get(), AbilitySystemComp.Get() );
			}
		}
	}

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
	if( AbilitySystemComp->HasMatchingGameplayTag( TAG_MovingLocked ) == true )
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

void AHeroCharacter::Input_AbilityInputTagPressed( FGameplayTag InputTag )
{
	const UInputAction* IA = InputConfig->FindAbilityInputActionForTag( InputTag, false );

	if( GetWorld()->IsPaused() && !IA->bTriggerWhenPaused )
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
	if( UARAbilitySystemComponent* ArASC = GetARAbilitySystemComponent() )
	{
		ArASC->AbilityInputTagReleased( InputTag );
	}
}

void AHeroCharacter::InitializerAttributes()
{
	Super::InitializerAttributes();

	if( !HealthAttribInitializer )
	{
		RLOG( Error, TEXT( "HealthAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle HealthHandle = AbilitySystemComp->MakeOutgoingSpec( HealthAttribInitializer, GetCharacterLevel(), EffectContext );
	if( HealthHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *HealthHandle.Data.Get(), AbilitySystemComp.Get() );
	}

	if( !AttackAttribInitializer )
	{
		RLOG( Error, TEXT( "AttackAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectSpecHandle AttackHandle = AbilitySystemComp->MakeOutgoingSpec( AttackAttribInitializer, GetCharacterLevel(), EffectContext );
	if( AttackHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *AttackHandle.Data.Get(), AbilitySystemComp.Get() );
	}

	if( !AgilityRefAttribInitializer )
	{
		RLOG( Error, TEXT( "AgilityRefAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectSpecHandle AGIHandle = AbilitySystemComp->MakeOutgoingSpec( AgilityRefAttribInitializer, GetCharacterLevel(), EffectContext );
	if( AGIHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *AGIHandle.Data.Get(), AbilitySystemComp.Get() );
	}

	if( !IntelligenceRefAttribInitializer )
	{
		RLOG( Error, TEXT( "IntelligenceRefAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectSpecHandle INTHandle = AbilitySystemComp->MakeOutgoingSpec( IntelligenceRefAttribInitializer, GetCharacterLevel(), EffectContext );
	if( INTHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *INTHandle.Data.Get(), AbilitySystemComp.Get() );
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

float AHeroCharacter::GetMaxHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxHealth();
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

float AHeroCharacter::GetMaxStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxStamina();
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
