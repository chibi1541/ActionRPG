// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ARTargetingComponent.h"

#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Ability/ActionRPGGlobalTags.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARTargetingComponent)


// Sets default values for this component's properties
UARTargetingComponent::UARTargetingComponent()
{
	auto Owner = Cast<ABaseCharacter>( GetOwner() );
	PrimaryComponentTick.bCanEverTick = ( Owner && Owner->GetCharacterType() == ECharacterType::CT_Hero );
}


// Called when the game starts
void UARTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetingIndex = 0;
	TargetList.Reset();

	const ABaseCharacter* Owner = Cast<ABaseCharacter>( GetOwner() );
	if( Owner )
	{
		if( GetWorld()->IsValidLowLevel() )
		{
			TArray<AActor*> arrCharacters;
			UGameplayStatics::GetAllActorsOfClass( GetWorld(), ABaseCharacter::StaticClass(), arrCharacters );
			for( AActor* FindOne : arrCharacters )
			{
				const ABaseCharacter* Target = Cast<ABaseCharacter>( FindOne );
				if( !Target->GetTargetingComponent() )
				{
					continue;
				}

				if( Target )
				{
					if( Target->GetCharacterType() != Owner->GetCharacterType() )
					{
						TargetList.Add( Target );
					}
					else if( Owner != Target )
					{
						AllianceList.Add( Target );
					}
				}
			}
		}
	}
}


// Called every frame
void UARTargetingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( TargetCharacter.IsValid() )
	{
		UpdateTargeting( DeltaTime );
	}
}

bool UARTargetingComponent::SetTargeting( bool bTargeting )
{
	if( bTargeting )
	{
		ArrageTargetList();

		auto Owner = Cast<ABaseCharacter>( GetOwner() );
		if( ( !TargetCharacter.IsValid() ) && Owner )
		{
			if( TargetList.Num() > 0 )
			{
				if( TargetingIndex >= TargetList.Num() )
				{
					TargetingIndex = 0;
				}

				TargetCharacter = TargetList[TargetingIndex];
			}

			if( TargetCharacter.IsValid() )
			{
				return true;
			}
		}
	}
	else
	{
		TargetCharacter.Reset();
		return true;
	}

	return false;
}

void UARTargetingComponent::ChangeTargeting( bool ReverseOrder )
{
	if( TargetList.Num() <= 1 )
	{
		return;
	}

	ArrageTargetList();
	if( ReverseOrder )
	{
		TargetingIndex--;
		if( TargetingIndex < 0 )
		{
			TargetingIndex = TargetList.Num() - 1;
		}
	}
	else
	{
		TargetingIndex++;
		if( TargetingIndex >= TargetList.Num() )
		{
			TargetingIndex = 0;
		}
	}

	TargetCharacter = TargetList[TargetingIndex];
}

const ABaseCharacter* UARTargetingComponent::GetTargetCharacter() const
{
	if( !TargetCharacter.IsValid() )
	{
		return nullptr;
	}

	return TargetCharacter.Get();
}

bool UARTargetingComponent::SetAllianceTargeting( bool bTargeting )
{
	if( bTargeting )
	{
		auto Owner = Cast<ABaseCharacter>( GetOwner() );
		if( ( !AllianceTargetCharacter.IsValid() ) && Owner )
		{
			if( AllianceList.Num() > 0 )
			{
				if( AllianceTargetingIndex >= AllianceList.Num() )
				{
					AllianceTargetingIndex = 0;
				}

				AllianceTargetCharacter = AllianceList[AllianceTargetingIndex];
			}

			if( AllianceTargetCharacter.IsValid() )
			{
				return true;
			}
		}
	}
	else
	{
		AllianceTargetCharacter.Reset();
		return true;
	}

	return false;
}

void UARTargetingComponent::ChangeAllianceTargeting( bool ReverseOrder )
{
	if( AllianceList.Num() <= 1 )
	{
		return;
	}

	if( ReverseOrder )
	{
		AllianceTargetingIndex--;
		if( AllianceTargetingIndex < 0 )
		{
			AllianceTargetingIndex = AllianceList.Num() - 1;
		}
	}
	else
	{
		AllianceTargetingIndex++;
		if( AllianceTargetingIndex >= AllianceList.Num() )
		{
			AllianceTargetingIndex = 0;
		}
	}

	AllianceTargetCharacter = AllianceList[AllianceTargetingIndex];
}

const ABaseCharacter* UARTargetingComponent::GetAllianceTargetCharacter() const
{
	if( !AllianceTargetCharacter.IsValid() )
	{
		return nullptr;
	}

	return AllianceTargetCharacter.Get();
}

void UARTargetingComponent::UpdateTargeting( float DeltaTime )
{
	if( !TargetCharacter.IsValid() )
	{
		const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( GetOwner(), Tags.AbilityStateTag_TargetingCancel, Payload );
		return;
	}

	auto Character = Cast<ABaseCharacter>( GetOwner() );
	FVector vecTarget = TargetCharacter->GetTargetingComponent()->GetComponentLocation();
	FVector vecDesired = vecTarget - Character->GetActorLocation();
	auto rotDesired = vecDesired.Rotation();
	FRotator rotCurrent = Character->GetControlRotation();
	rotDesired = FRotator( -rotDesired.Pitch, rotDesired.Yaw, 0.0f );

	Character->GetController()->SetControlRotation(
		FMath::RInterpTo( rotCurrent, rotDesired, DeltaTime, 10.0f ) );

	const FActionRPGGlobalTags& Tags = FActionRPGGlobalTags::Get();
	const auto ASC = Character->GetAbilitySystemComponent();
	if( ASC && !ASC->HasMatchingGameplayTag( Tags.AbilityStateTag_Sprint ) )
	{
		Character->SetActorRotation( FRotator( 0.f, rotDesired.Yaw, 0.f ) );
	}
}

void UARTargetingComponent::ArrageTargetList()
{
	for( auto Target : TargetList )
	{
		if( Target.IsValid() == false )
		{
			TargetList.Remove( Target );
		}
	}
}

