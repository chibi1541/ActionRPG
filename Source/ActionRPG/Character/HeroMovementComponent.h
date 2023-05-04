// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "HeroMovementComponent.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UHeroMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroMovementComponent();

protected:
	virtual void BeginPlay() override;

protected:

	void Move( const FInputActionValue& Value );

	void Look( const FInputActionValue& Value );

public:
	template <class T>
	T* GetPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawn must be derived from APawn");
		return Cast<T>(GetOwner());
	}

	template <class T>
	T* GetPawnChecked() const
	{
		static_assert( TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawnChecked must be derived from APawn" );
		return CastChecked<T>( GetOwner() );
	}

	template <class T>
	T* GetController() const
	{
		static_assert( TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template parameter to GetController must be derived from AController" );
		return GetPawnChecked<APawn>()->GetController<T>();
	}

protected:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr<UInputAction> LookAction;
};
