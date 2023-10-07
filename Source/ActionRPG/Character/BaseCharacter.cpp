// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Character/Components/ARCharacterStateComponent.h"
#include "Ability/AbilitySet.h"
#include "Ability/ARAbilitySystemComponent.h"
#include "Character/ARMovementComponent.h"
#include "Character/Attribute/ARBaseAttribSet.h"
#include "Character/Components/ARTargetingComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCharacter)

ABaseCharacter::ABaseCharacter( const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get() */ )
	:Super( ObjectInitializer.SetDefaultSubobjectClass<UARMovementComponent>( ACharacter::CharacterMovementComponentName ) )
{
	CharacterStateComponent = CreateDefaultSubobject<UARCharacterStateComponent>( TEXT( "STATECOMP" ) );

	AbilitySystemComp = CharacterStateComponent->GetARAbilitySystemComponent();

	AbilitySystemComp.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	BaseAttribSet = CreateDefaultSubobject<UARBaseAttribSet>( TEXT( "ARBaseAttribSet" ) );

	TargetingComponent = CreateDefaultSubobject<UARTargetingComponent>( TEXT( "TARGETINGCOMP" ) );
	TargetingComponent->SetupAttachment( RootComponent );
}

class UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return GetARAbilitySystemComponent();
}

void ABaseCharacter::GetOwnedGameplayTags( FGameplayTagContainer& TagContainer ) const
{
	AbilitySystemComp->GetOwnedGameplayTags( TagContainer );
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( AbilitySystemComp )
	{
		AbilitySystemComp->InitAbilityActorInfo( this, this );
		InitAbilitySystem();

		InitializerAttributes();
	}
}

UARCharacterStateComponent* ABaseCharacter::GetCharacterStateComponenet() const
{
	return CharacterStateComponent;
}

UARTargetingComponent* ABaseCharacter::GetTargetingComponent() const
{
	if( CharacterType == ECharacterType::CT_Hero
		|| CharacterType == ECharacterType::CT_Monster )
	{
		return TargetingComponent;
	}

	return nullptr;
}

FVector ABaseCharacter::GetCurrentVelocity() const
{
	const auto MovementComponent = GetMovementComponent();
	if( !MovementComponent )
	{
		return FVector::Zero();
	}

	FVector Velocity = MovementComponent->Velocity;
	if( Velocity.Size2D() == 0.f )
	{
		return FVector::Zero();
	}

	const FRotator Rotation = Controller->GetControlRotation();
	float Yaw = 360.f - Rotation.Yaw;
	Velocity = Velocity.RotateAngleAxis( Yaw, FVector::UpVector );

	return Velocity;
}

void ABaseCharacter::InitAbilitySystem()
{
	for( const UAbilitySet* AbilitySet : AbilitySets )
	{
		if( AbilitySet != nullptr )
		{
			AbilitySet->GiveToAbilitySystem( AbilitySystemComp );
		}
	}
}

void ABaseCharacter::InitializerAttributes()
{
	if( !BaseAttribInitializer )
	{
		RLOG( Error, TEXT( "BaseAttribInitializer is Missing : %s" ), *GetName() );
		return;
	}

	FGameplayEffectContextHandle BaseEffectContext = AbilitySystemComp->MakeEffectContext();
	BaseEffectContext.AddSourceObject( this );

	FGameplayEffectSpecHandle BaseHandle = AbilitySystemComp->MakeOutgoingSpec( BaseAttribInitializer, GetCharacterLevel(), BaseEffectContext );
	if( BaseHandle.IsValid() )
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget( *BaseHandle.Data.Get(), AbilitySystemComp.Get() );
	}
}

int32 ABaseCharacter::GetCharacterLevel() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetCharacterLevel() );
	}

	return 0;
}

int32 ABaseCharacter::GetStrength() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetStrength() );
	}

	return 0;
}

int32 ABaseCharacter::GetAgility() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetAgility() );
	}

	return 0;
}

int32 ABaseCharacter::GetIntelligence() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetIntelligence() );
	}

	return 0;
}

int32 ABaseCharacter::GetVitality() const
{
	if( BaseAttribSet )
	{
		return static_cast< int32 >( BaseAttribSet->GetVitality() );
	}

	return 0;
}
