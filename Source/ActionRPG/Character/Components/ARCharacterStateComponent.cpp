// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARCharacterStateComponent.h"

#include "Ability/ARAbilitySystemComponent.h"
#include "Character/Components/GetHitComponent.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "ARGASEnumDef.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Character/Attribute/ARVitRefAttribSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARCharacterStateComponent)

// Sets default values for this component's properties
UARCharacterStateComponent::UARCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	StiffEffectSpecHandle = FGameplayEffectSpecHandle( nullptr );

	AbilitySystemComponent = CreateDefaultSubobject<UARAbilitySystemComponent>( TEXT( "ARASC" ) );
	AbilitySystemComponent.Get()->ReplicationMode = EGameplayEffectReplicationMode::Full;

	IsStunned = false;
}

UAbilitySystemComponent* UARCharacterStateComponent::GetAbilitySystemComponent() const
{
	return GetARAbilitySystemComponent();
}

// Called when the game starts
void UARCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();
	if( !Owner )
	{
		RLOG( Error, TEXT( "Owner is not Valid" ) );
		return;
	}

	GetHitComp = Cast<UGetHitComponent>( Owner->GetComponentByClass( UGetHitComponent::StaticClass() ) );

	HealthAttrib = AbilitySystemComponent->GetSet<UARVitRefAttribSet>();

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnGetStuned );
	AbilitySystemComponent->GameplayEffectRemoveCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Stun ).AddDynamic( this, &UARCharacterStateComponent::OnStunStateRemoved );
	AbilitySystemComponent->ActiveGameplayEffectCallBacks.FindOrAdd( EGameplayEffectDelegateType::EDT_Provoke ).AddDynamic( this, &UARCharacterStateComponent::OnProvoked );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttrib->GetStaminaAttribute() ).AddUObject( this, &UARCharacterStateComponent::OnStaminaChange );
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

void UARCharacterStateComponent::OnRefreshProvoked( FActiveGameplayEffect& EffectSpec )
{

}

void UARCharacterStateComponent::OnStaminaChange( const FOnAttributeChangeData& Data )
{
	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();

	if( Data.NewValue == 0.f )
	{
		AbilitySystemComponent->AddLooseGameplayTag( Tags.CharacterStateTag_Exhausted );
		return;
	}
	else if( Data.OldValue == 0.f )
	{
		AbilitySystemComponent->RemoveLooseGameplayTag( Tags.CharacterStateTag_Exhausted );
		return;
	}

	if( Data.NewValue > Data.OldValue && Data.NewValue >= HealthAttrib->GetMaxStamina() )
	{
		AbilitySystemComponent->AddLooseGameplayTag( Tags.CharacterStateTag_FullStamina );
	}
	else if( Data.NewValue < HealthAttrib->GetMaxStamina() && AbilitySystemComponent->HasMatchingGameplayTag( Tags.CharacterStateTag_FullStamina ) )
	{
		AbilitySystemComponent->RemoveLooseGameplayTag( Tags.CharacterStateTag_FullStamina );
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

const bool UARCharacterStateComponent::GetStunState() const
{
	return IsStunned;
}
