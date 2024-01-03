// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UObject/ObjectMacros.h"
#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include "ARExtensionPointWidget.generated.h"

class ULocalPlayer;


UCLASS()
class ACTIONRPG_API UARExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam( TSubclassOf<UUserWidget>, FOnGetWidgetClassForData, UObject*, DataItem );
	DECLARE_DYNAMIC_DELEGATE_TwoParams( FOnConfigureWidgetForData, UUserWidget*, Widget, UObject*, DataItem );

	UARExtensionPointWidget( const FObjectInitializer& ObjectInitializer );

	virtual void ReleaseSlateResources( bool bReleaseChildren ) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	
	void ResetExtensionPoint();
	void RegisterExtensionPoint();
	void OnAddOrRemoveExtension( EExtensionAction Action, const FUIExtensionRequest& Request );

protected:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "UI Extension" )
		FGameplayTag ExtensionPointTag;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "UI Extension" )
		EExtensionPointMatch ExtensionPointTagMatch = EExtensionPointMatch::ExactMatch;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "UI Extension" )
		TArray<TObjectPtr<UClass>> DataClasses;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "UI Extension", meta = ( IsBindableEvent = "True" ) )
		FOnGetWidgetClassForData GetWidgetClassForData;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "UI Extension", meta = ( IsBindableEvent = "True" ) )
		FOnConfigureWidgetForData ConfigureWidgetForData;

	FUIExtensionPointHandle ExtensionPointHandle;

	UPROPERTY( Transient )
		TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;
};
