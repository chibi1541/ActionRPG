// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseMonster.h"

#include "Character/BaseAIController.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARAttackAttribSet.h"
#include "Character/Attribute/ARAgiRefAttribSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UserInterface/ARMonsterHPBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseMonster)

ABaseMonster::ABaseMonster( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	AIControllerClass = ABaseAIController::StaticClass();

	AttackAttribSet = CreateDefaultSubobject<UARAttackAttribSet>( "ARAttackAttribSet" );
	VitRefAttribSet = CreateDefaultSubobject<UARVitRefAttribSet>( "ARVirRefAttribSet" );
	AgiRefAttribSet = CreateDefaultSubobject<UARAgiRefAttribSet>( "UARAgiRefAttribSet" );

	FloatingHPBarComponent = CreateDefaultSubobject<UWidgetComponent>( FName( "FLOATHPBARCOMP" ) );
	FloatingHPBarComponent->SetupAttachment( RootComponent );
	FloatingHPBarComponent->SetRelativeLocation( FVector( 0, 0, 120 ) );
	FloatingHPBarComponent->SetWidgetSpace( EWidgetSpace::Screen );
	FloatingHPBarComponent->SetDrawSize( FVector2D( 80.f, 8.f ) );
}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	SetHealth( GetMaxHealth() );
	SetStamina( GetMaxStamina() );
	SetShieldGauge( GetMaxShieldGauge() );

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if( AgiRefAttribSet )
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate( AgiRefAttribSet->GetModifiedMoveSpeedAttribute() ).AddUObject( this, &ABaseMonster::OnSpeedChange );
	}


	APlayerController* PlayerController = UGameplayStatics::GetPlayerController( GetWorld(), 0 );

	if( FloatingHPBarClass )
	{
		FloatingHPBar = CreateWidget<UARMonsterHPBarWidget>( PlayerController, FloatingHPBarClass );
		if( FloatingHPBar && FloatingHPBarComponent )
		{
			FloatingHPBarComponent->SetWidget( FloatingHPBar );

			FloatingHPBar->InitializeWidget( CharacterStateComponent );
		}
	}
}

void ABaseMonster::SetHealth( float Health )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetHealth( Health );
	}
}

void ABaseMonster::SetStamina( float Stamina )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetStamina( Stamina );
	}
}

void ABaseMonster::SetShieldGauge( float ShieldGauge )
{
	if( VitRefAttribSet )
	{
		VitRefAttribSet->SetShieldGauge( ShieldGauge );
	}
}

void ABaseMonster::FinishDying()
{
	Super::FinishDying();

	Destroy();
}

void ABaseMonster::Die()
{
	Super::Die();

	if( DyingMontages.Num() > 0 )
	{
		int DyingMotionIndex = FMath::RandRange( 0, DyingMontages.Num() - 1 );

		GetMesh()->GetAnimInstance()->Montage_Play( DyingMontages[DyingMotionIndex] );
	}

	GetCapsuleComponent()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	GetMesh()->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector( 0 );
}

float ABaseMonster::GetCurrentHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetHealth();
	}

	return 0.f;
}

float ABaseMonster::GetCurrentStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetStamina();
	}

	return 0.f;
}

float ABaseMonster::GetMaxHealth() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxHealth();
	}

	return 0.f;
}

float ABaseMonster::GetMaxStamina() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxStamina();
	}

	return 0.f;
}

float ABaseMonster::GetMaxShieldGauge() const
{
	if( VitRefAttribSet )
	{
		return VitRefAttribSet->GetMaxShieldGauge();
	}

	return 0.f;
}

float ABaseMonster::GetMoveSpeed() const
{
	if( AgiRefAttribSet )
	{
		return AgiRefAttribSet->GetModifiedMoveSpeed();
	}

	return 0.f;
}
