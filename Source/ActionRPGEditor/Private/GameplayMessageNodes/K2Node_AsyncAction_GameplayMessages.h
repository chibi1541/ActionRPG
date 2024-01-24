// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "K2Node_AsyncAction.h"
#include "Containers/Array.h"
#include "UObject/UObjectGlobals.h"

#include "K2Node_AsyncAction_GameplayMessages.generated.h"

class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;
class FMulticastDelegateProperty;
class FString;
class UEdGraph;
class UEdGraphPin;
class UObject;

/**
 * 
 */
UCLASS()
class UK2Node_AsyncAction_GameplayMessages : public UK2Node_AsyncAction
{
	GENERATED_BODY()
	
	virtual void PostReconstructNode() override;
	virtual void PinDefaultValueChanged( UEdGraphPin* ChangedPin ) override;
	virtual void GetPinHoverText( const UEdGraphPin& Pin, FString& HoverTextOut ) const override;

	virtual void GetMenuActions( FBlueprintActionDatabaseRegistrar& ActionRegistrar ) const override;
	virtual void AllocateDefaultPins() override;

protected:
	virtual bool HandleDelegates(
		const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin,
		UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext ) override;

private:

	// Add the GetPayload flow to the end of the delegate handler's logic chain
	bool HandlePayloadImplementation(
		FMulticastDelegateProperty* CurrentProperty,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ProxyObjectVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& PayloadVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ActualChannelVar,
		UEdGraphPin*& InOutLastActivatedThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext );

	void RefreshOutputPayloadType();

	UEdGraphPin* GetPayloadPin() const;
	UEdGraphPin* GetPayloadTypePin() const;
	UEdGraphPin* GetOutputChannelPin() const;

};
