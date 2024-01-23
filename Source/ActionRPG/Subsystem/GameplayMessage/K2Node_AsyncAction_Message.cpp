// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameplayMessage/K2Node_AsyncAction_Message.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "BlueprintNodeSpawner.h"
#include "Containers/EnumAsByte.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "Engine/MemberReference.h"
#include "HAL/Platform.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Text.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CallFunction.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "Templates/SubclassOf.h"
#include "UObject/Class.h"
#include "UObject/Field.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UnrealType.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "Subsystem/GameplayMessage/AsyncAction_GameplayMessage.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(K2Node_AsyncAction_Message)

class UEdGraph;

#define LOCTEXT_NAMESPACE "K2Node"

namespace UK2Node_AsyncAction_MessageHelper
{
	static FName ActualChannelPinName = "ActualChannel";
	static FName PayloadPinName = "Payload";
	static FName PayloadTypePinName = "PayloadType";
	static FName DelegateProxyPinName = "ProxyObject";
};