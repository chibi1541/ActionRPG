// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/SceneComponent.h"

#include "ARTargetingComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class ACTIONRPG_API UARTargetingComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UARTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	
public:
	// Enemy
	UFUNCTION( BlueprintCallable, Category = "CharacterState|Targeting" )
		bool SetTargeting( bool bTargeting );

	UFUNCTION( BlueprintCallable, Category = "CharacterState|Targeting" )
		void ChangeTargeting( bool ReverseOrder );

	UFUNCTION( BlueprintCallable, Category = "CharacterState|Targeting" )
		const ABaseCharacter* GetTargetCharacter() const;

private:
	void UpdateTargeting( float DeltaTime );

	void ArrageTargetList();

private:

	// Enemy
	TWeakObjectPtr<const ABaseCharacter> TargetCharacter;

	TArray<TWeakObjectPtr<const ABaseCharacter>> TargetList;

	int TargetingIndex;

};
