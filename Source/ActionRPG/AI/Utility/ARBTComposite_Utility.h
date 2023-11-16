// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "AI/Utility/ARBTUtilityTypes.h"

#include "ARBTComposite_Utility.generated.h"

UENUM()
enum class EUtilitySelectionMethodType : uint8
{
	// The child with the highest utility value is always chosen
	Priority,

	// Selection probability is proportional to utility value
	Proportional,
};

struct FARBTUtilityMemory : public FBTCompositeMemory
{
	/** ordering of child nodes for the current search activation (array is a list of child indexes arranged in execution order) */
	FUtilityExecutionOrdering ExecutionOrdering;
};

class UBTDecorator_UtilityFunction;

/**
 *
 */
UCLASS()
class ACTIONRPG_API UARBTComposite_Utility : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

public:
	// Method used to determine child selection based on utility values
	UPROPERTY( EditAnywhere, Category = "Utility" )
		EUtilitySelectionMethodType SelectionMethodType;

public:
	virtual void InitializeMemory( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType ) const override;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

protected:

	//const UBTDecorator_UtilityFunction* FindChildUtilityFunction( int32 ChildIndex ) const;

	virtual void NotifyNodeActivation( FBehaviorTreeSearchData& SearchData ) const override;


};
