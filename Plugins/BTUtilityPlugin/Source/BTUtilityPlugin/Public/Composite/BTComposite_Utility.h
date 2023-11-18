// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BTUtilityTypes.h"
#include "BehaviorTree/BTCompositeNode.h"

#include "BTComposite_Utility.generated.h"

UENUM()
enum class EUtilitySelectionMethodType : uint8
{
	// The child with the highest utility value is always chosen
	Priority,

	// Selection probability is proportional to utility value
	Proportional,
};

struct FBTUtilityMemory : public FBTCompositeMemory
{
	/** ordering of child nodes for the current search activation (array is a list of child indexes arranged in execution order) */
	FUtilityExecutionOrdering ExecutionOrdering;
};

class UBTDecorator_UtilityFunction;

/**
 * 
 */
UCLASS()
class BTUTILITYPLUGIN_API UBTComposite_Utility : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeMemory( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType ) const override;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual int32 GetNextChildHandler( struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult ) const override;

	const UBTDecorator_UtilityFunction* FindChildUtilityFunction( int32 ChildIndex ) const;

	virtual void NotifyNodeActivation( FBehaviorTreeSearchData& SearchData ) const override;

	bool EvaluateUtilityScores( FBehaviorTreeSearchData& SearchData, OUT TArray<float>& Scores ) const;


public:
	// Method used to determine child selection based on utility values
	UPROPERTY( EditAnywhere, Category = "Utility" )
		EUtilitySelectionMethodType SelectionMethodType;
};
