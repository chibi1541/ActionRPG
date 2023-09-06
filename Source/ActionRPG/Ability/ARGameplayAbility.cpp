// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/ARGameplayAbility.h"

#include "AbilitySystemGlobals.h"
#include "Ability/ARAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameplayAbility)

UARGameplayAbility::UARGameplayAbility( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
	{
		static FName PressFuncName = FName( TEXT( "K2_InputPressed" ) );
		UFunction* InputPressFunction = GetClass()->FindFunctionByName( PressFuncName );
		bInputPressBlueprintCanUse = InputPressFunction && ensure( InputPressFunction->GetOuter() ) && InputPressFunction->GetOuter()->IsA( UBlueprintGeneratedClass::StaticClass() );

	}

	{
		static FName ReleaseFuncName = FName( TEXT( "K2_InputReleased" ) );
		UFunction* InputReleaseFunction = GetClass()->FindFunctionByName( ReleaseFuncName );
		bInputReleaseBlueprintCanUse = InputReleaseFunction && ensure( InputReleaseFunction->GetOuter() ) && InputReleaseFunction->GetOuter()->IsA( UBlueprintGeneratedClass::StaticClass() );
	}

	bActivateAbilityOnGranted = false;
}

bool UARGameplayAbility::DoesAbilitySatisfyTagRequirements( const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags ) const
{
	bool bBlocked = false;
	bool bMissing = false;

	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	// Check if any of this ability's tags are currently blocked
	if( AbilitySystemComponent.AreAbilityTagsBlocked( AbilityTags ) )
	{
		bBlocked = true;
	}

	const UARAbilitySystemComponent* ArASC = Cast<UARAbilitySystemComponent>( &AbilitySystemComponent );
	FGameplayTagContainer ModiActivationBlockedTags = ActivationBlockedTags;
	FGameplayTagContainer ModiActivationRequiredTags = ActivationRequiredTags;

	if( ArASC != nullptr )
	{
		ArASC->GetAdditionalActivationTagRequirements( AbilityTags, ModiActivationRequiredTags, ModiActivationBlockedTags );
	}

	// Check to see the required/blocked tags for this ability
	if( ModiActivationRequiredTags.Num() || ModiActivationBlockedTags.Num() )
	{
		static FGameplayTagContainer AbilitySystemComponentTags;
		AbilitySystemComponentTags.Reset();

		AbilitySystemComponent.GetOwnedGameplayTags( AbilitySystemComponentTags );

		if( AbilitySystemComponentTags.HasAny( ModiActivationBlockedTags ) )
		{
			bBlocked = true;
		}

		if( !AbilitySystemComponentTags.HasAll( ModiActivationRequiredTags ) )
		{
			bMissing = true;
		}
	}

	if( SourceTags != nullptr )
	{
		if( SourceBlockedTags.Num() || SourceRequiredTags.Num() )
		{
			if( SourceTags->HasAny( SourceBlockedTags ) )
			{
				bBlocked = true;
			}

			if( !SourceTags->HasAll( SourceRequiredTags ) )
			{
				bMissing = true;
			}
		}
	}

	if( TargetTags != nullptr )
	{
		if( TargetBlockedTags.Num() || TargetRequiredTags.Num() )
		{
			if( TargetTags->HasAny( TargetBlockedTags ) )
			{
				bBlocked = true;
			}

			if( !TargetTags->HasAll( TargetRequiredTags ) )
			{
				bMissing = true;
			}
		}
	}

	if( bBlocked )
	{
		if( OptionalRelevantTags && BlockedTag.IsValid() )
		{
			OptionalRelevantTags->AddTag( BlockedTag );
		}
		return false;
	}
	if( bMissing )
	{
		if( OptionalRelevantTags && MissingTag.IsValid() )
		{
			OptionalRelevantTags->AddTag( MissingTag );
		}
		return false;
	}

	return true;
}

void UARGameplayAbility::OnAvatarSet( const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec )
{
	Super::OnAvatarSet( ActorInfo, Spec );

	if( bActivateAbilityOnGranted )
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility( Spec.Handle, false );
	}
}

void UARGameplayAbility::InputReleased( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
{
	if( ActionPolicyType != EARAbilityActivationPolicy::WhileInputHeld )
		return;

	if( OnReleased.IsBound() )
	{
		RLOG( Warning, TEXT( "Call Input Released Delegate!" ) );

		OnReleased.Broadcast();
	}

	if( bInputReleaseBlueprintCanUse )
	{
		if( K2_InputReleased( Handle, *ActorInfo, ActivationInfo ) == false )
		{
			RLOG( Warning, TEXT( "Call Input Release Error!" ) );
		}
	}
	else
	{
		RLOG( Warning, TEXT( "Can't Call Input Release!" ) );
	}

}

void UARGameplayAbility::InputPressed( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo )
{
	RLOG( Warning, TEXT( "Call Input Press" ) );


	if( OnPressed.IsBound() )
	{
		RLOG( Warning, TEXT( "Call Input Pressed Delegate!" ) );

		OnPressed.Broadcast();
	}


	if( bInputPressBlueprintCanUse )
	{
		if( K2_InputPressed( Handle, *ActorInfo, ActivationInfo ) == false )
		{
			RLOG( Warning, TEXT( "Call Input Press Error!" ) );
		}
	}
}

