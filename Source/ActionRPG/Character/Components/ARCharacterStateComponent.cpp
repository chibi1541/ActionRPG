// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARCharacterStateComponent.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Components/GetHitComponent.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "ARGASEnumDef.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Character/Attribute/ARVitRefAttribSet.h"
#include "Character/Attribute/ARIntRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARCharacterStateComponent)

// Sets default values for this component's properties
UARCharacterStateComponent::UARCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	StiffEffectSpecHandle = FGameplayEffectSpecHandle( nullptr );

	AbilitySystemComponent = CreateDefaultSubobject<UARAbilitySystemComponent>( TEXT( "ARASC" ) );
	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	IsStunned = false;

	IsDead = false;
}

UAbilitySystemComponent* UARCharacterStateComponent::GetAbilitySystemComponent() const
{
	return GetARAbilitySystemComponent();
}

// Called when the game starts
void UARCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	const ABaseCharacter* Owner = Cast<ABaseCharacter>( GetOwner() );
	if( !Owner )
	{
		RLOG( Error, TEXT( "Owner is not Valid" ) );
		return;
	}

	GetHitComp = Cast<UGetHitComponent>( Owner->GetComponentByClass( UGetHitComponent::StaticClass() ) );

	HealthAttrib = AbilitySystemComponent->GetSet<UARVitRefAttribSet>();
	ManaAttrib = AbilitySystemComponent->GetSet<UARIntRefAttribSet>();

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnGetStuned );
	AbilitySystemComponent->GameplayEffectRemoveCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnStunStateRemoved );
	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Provoke ).AddDynamic( this, &UARCharacterStateComponent::OnProvoked );

	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Guard ).AddDynamic( this, &UARCharacterStateComponent::OnGuard );
	AbilitySystemComponent->GameplayEffectRemoveCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Guard ).AddDynamic( this, &UARCharacterStateComponent::OnGuardRemoved );

	if( HealthAttrib.IsValid() )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetStaminaAttribute() ).AddUObject( this, &UARCharacterStateComponent::OnStaminaChange );
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetHealthAttribute() ).AddUObject( this, &UARCharacterStateComponent::OnHealthChange );

		if( HealthAttrib->GetMaxShieldGauge() == 0.f )
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetShieldGaugeAttribute() ).AddUObject( this, &UARCharacterStateComponent::OnShieldGaugeChange );
		}
	}

	if( ManaAttrib.IsValid() )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttrib->GetManaAttribute() ).AddUObject( this, &UARCharacterStateComponent::OnManaChange );
	}
}

void UARCharacterStateComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UARCharacterStateComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	if( StiffEffectSpecHandle.IsValid() )
	{
		StiffEffectSpecHandle = FGameplayEffectSpecHandle( nullptr );
	}
}

void UARCharacterStateComponent::OnGetStuned( const FGameplayEffectSpec& EffectSpec )
{
	IsStunned = true;
}

void UARCharacterStateComponent::OnStunStateRemoved( const FActiveGameplayEffect& ActiveEffect )
{
	IsStunned = false;
}

void UARCharacterStateComponent::OnProvoked( const FGameplayEffectSpec& EffectSpec )
{
	auto Owner = Cast<ABaseCharacter>( GetOwner() );
	auto AIController = Cast<ABaseAIController>( Owner->GetController() );
	if( !AIController )
	{
		RLOG( Error, TEXT( "Can't Find AIController : " ), *Owner->GetName() );
	}

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if( !Blackboard )
	{
		RLOG( Error, TEXT( "Can't Find BlackboardComponent" ), *Owner->GetName() );
	}

	Blackboard->SetValueAsObject( ABaseAIController::TargetKey, EffectSpec.GetEffectContext().GetInstigator() );
}

void UARCharacterStateComponent::OnGuard( const FGameplayEffectSpec& EffectSpec )
{
	IsGuard = true;
}

void UARCharacterStateComponent::OnGuardRemoved( const FActiveGameplayEffect& ActiveEffect )
{
	IsGuard = false;
}

void UARCharacterStateComponent::OnStaminaChange( const FOnAttributeChangeData& Data )
{
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	if( Data.NewValue == 0.f )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_Exhausted, 1 );
	}
	else if( Data.OldValue == 0.f )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_Exhausted, 0 );
	}

	if( Data.NewValue > Data.OldValue && Data.NewValue >= HealthAttrib->GetMaxStamina() )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullStamina, 1 );
	}
	else if( Data.NewValue < HealthAttrib->GetMaxStamina() && AbilitySystemComponent->HasMatchingGameplayTag( Tags.CharacterStateTag_FullStamina ) )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullStamina, 0 );
	}
}

void UARCharacterStateComponent::OnHealthChange( const FOnAttributeChangeData& Data )
{
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	if( Data.NewValue > Data.OldValue && Data.NewValue >= HealthAttrib->GetMaxHealth() )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullHealth, 1 );
	}
	else if( Data.NewValue < HealthAttrib->GetMaxHealth() && AbilitySystemComponent->HasMatchingGameplayTag( Tags.CharacterStateTag_FullHealth ) )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullHealth, 0 );
	}

	// Change Dead State
	if( Data.NewValue <= 0.f && Data.OldValue > 0.f )
	{
		IsDead = true;
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_Dead, 1 );
		AbilitySystemComponent->CancelAllAbilities();
	}
	else if( Data.NewValue > 0.f && IsDead )
	{
		IsDead = false;
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_Dead, 0 );
	}
}

void UARCharacterStateComponent::OnManaChange( const FOnAttributeChangeData& Data )
{
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	if( Data.NewValue > Data.OldValue && Data.NewValue >= ManaAttrib->GetMaxMana() )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullMana, 1 );
	}
	else if( Data.NewValue < ManaAttrib->GetMaxMana() && AbilitySystemComponent->HasMatchingGameplayTag( Tags.CharacterStateTag_FullMana ) )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullMana, 0 );
	}
}

void UARCharacterStateComponent::OnShieldGaugeChange( const FOnAttributeChangeData& Data )
{
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	if( HealthAttrib->GetMaxShieldGauge() > 0.f && Data.NewValue == 0.f )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_ShieldEmpty, 1 );
	}
	else if( HealthAttrib->GetMaxShieldGauge() > 0.f && Data.OldValue == 0.f )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_ShieldEmpty, 0 );
	}


	if( Data.NewValue > Data.OldValue && Data.NewValue >= HealthAttrib->GetMaxShieldGauge() )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullShield, 1 );
	}
	else if( Data.NewValue < HealthAttrib->GetMaxShieldGauge() && AbilitySystemComponent->HasMatchingGameplayTag( Tags.CharacterStateTag_FullShield ) )
	{
		AbilitySystemComponent->SetLooseGameplayTagCount( Tags.CharacterStateTag_FullShield, 0 );
	}
}

void UARCharacterStateComponent::SetStiffEffectSpec( const FGameplayEffectSpecHandle& SpecHandle )
{
	if( !SpecHandle.IsValid() )
	{
		RLOG( Error, TEXT( "Stiff SpecHandle is not Valid : %s" ), *GetOwner()->GetName() );
	}

	StiffEffectSpecHandle = SpecHandle;

	if( !GetHitComp.IsValid() )
	{
		RLOG( Error, TEXT( "Stiff Effect Activation is Failed : %s" ), *GetOwner()->GetName() );
		return;
	}

	float EffectDuration = GetHitComp->GetReactionMontagePlayTime( SpecHandle.Data->GetEffectContext().GetHitResult()->Normal );
	StiffEffectSpecHandle.Data->SetDuration( EffectDuration, false );

	FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget( *StiffEffectSpecHandle.Data.Get(), AbilitySystemComponent );
}

bool UARCharacterStateComponent::GetStunState() const
{
	return IsStunned;
}

bool UARCharacterStateComponent::GetDeadState() const
{
	return IsDead;
}

bool UARCharacterStateComponent::GetGuardState() const
{
	return IsGuard;
}
