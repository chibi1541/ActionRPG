// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"


#include "ARGameUIManagerSubsystem.generated.h"

class UUserWidget;
class UARPrimaryGameLayout;
class ULocalPlayer;
class UCommonActivatableWidget;

UENUM( BlueprintType )
enum class EExtensionPointMatch : uint8
{
	ExactMatch,
	PartialMatch
};

UENUM( BlueprintType )
enum class EExtensionAction : uint8
{
	Added,
	Removed
};

struct FUIExtension : TSharedFromThis<FUIExtension>
{

public:
	FGameplayTag ExtensionPointTag;
	TSubclassOf<UUserWidget> WidgetClass;
	UObject* Data = nullptr;
};

DECLARE_DELEGATE_TwoParams( FExtendExtensionPointDelegate, EExtensionAction Action, const FUIExtensionRequest& Request );


struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{

public:

	FGameplayTag ExtensionPointTag;
	EExtensionPointMatch TagMatchType = EExtensionPointMatch::ExactMatch;
	TArray<UClass*> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	// bool DoesExtensionPassContract( const FUIExtension* Extension ) const;
};

USTRUCT( BlueprintType )
struct FUIExtensionPointHandle
{
	GENERATED_BODY()

public:
	FUIExtensionPointHandle() {}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

private:
	TWeakObjectPtr<UARGameUIManagerSubsystem> UIManager;

	TSharedPtr<FUIExtensionPoint> DataPtr;

	friend UARGameUIManagerSubsystem;

	FUIExtensionPointHandle( UARGameUIManagerSubsystem* ManagerSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr ) : UIManager( ManagerSource ), DataPtr( InDataPtr ) {}
};

USTRUCT( BlueprintType )
struct ACTIONRPG_API FUIExtensionHandle
{
	GENERATED_BODY()

public:
	FUIExtensionHandle() {}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==( const FUIExtensionHandle& Other ) const { return DataPtr == Other.DataPtr; }
	bool operator!=( const FUIExtensionHandle& Other ) const { return !operator==( Other ); }

	friend FORCEINLINE uint32 GetTypeHash( const FUIExtensionHandle& Handle )
	{
		return PointerHash( Handle.DataPtr.Get() );
	}

private:
	TWeakObjectPtr<UARGameUIManagerSubsystem> UIManager;

	TSharedPtr<FUIExtension> DataPtr;

	friend UARGameUIManagerSubsystem;

	FUIExtensionHandle( UARGameUIManagerSubsystem* ManagerSource, const TSharedPtr<FUIExtension>& InDataPtr ) : UIManager( ManagerSource ), DataPtr( InDataPtr ) {}
};

USTRUCT( BlueprintType )
struct FUIExtensionRequest
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		FUIExtensionHandle ExtensionHandle;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		FGameplayTag ExtensionPointTag;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		TObjectPtr<UObject> Data = nullptr;
};

UCLASS()
class ACTIONRPG_API UARGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	UARGameUIManagerSubsystem();

	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	virtual void Deinitialize() override;

	TObjectPtr<UARPrimaryGameLayout> GetLayoutWidgetClass();

	void CreatePrimaryLayoutWidget( const ULocalPlayer* LocalPlayer );

	void AddMainGameHUDWidget();

	FUIExtensionPointHandle RegisterExtensionPoint( const FGameplayTag& ExtensionPointTag, EExtensionPointMatch TagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback );

	UFUNCTION( BlueprintCallable )
		FUIExtensionHandle RegisterExtensionWidget( const FGameplayTag& ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* Data );


	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		void UnregisterExtensionPoint( const FGameplayTag& ExtensionPointTag );

	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		void UnregisterExtension( const FUIExtensionHandle& ExtensionHandle );

	UFUNCTION( BlueprintCallable, Category = "LayoutWidget" )
		void AddLayerWidget( const FGameplayTag& LayerTag, TSoftClassPtr<UCommonActivatableWidget> LayerClass );


	void RemoveAllLayouts();

protected:

	void NotifyRegisterExtensionPoint( TSharedPtr<FUIExtensionPoint>& ExtensionPoint );

	void NotifyRegisterExtensionWidget( EExtensionAction Action, TSharedPtr<FUIExtension>& Extension );

private:
	UPROPERTY( Transient )
		TObjectPtr<UARPrimaryGameLayout> PrimaryLayout;

	TArray<TWeakObjectPtr<UCommonActivatableWidget>> Layouts;

	TMap<FGameplayTag, TSharedPtr<FUIExtensionPoint>> ExtensionPointMap;

	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
};


UCLASS()
class ACTIONRPG_API UUIExtensionHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UUIExtensionHandleFunctions() {}

	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		static void Unregister( UPARAM( ref ) FUIExtensionHandle& Handle );

	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		static bool IsValid( UPARAM( ref ) FUIExtensionHandle& Handle );
};

UCLASS()
class ACTIONRPG_API UUIExtensionPointHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UUIExtensionPointHandleFunctions() {}

	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		static void Unregister( UPARAM( ref ) FUIExtensionPointHandle& Handle );

	UFUNCTION( BlueprintCallable, BlueprintCosmetic, Category = "UI Extension" )
		static bool IsValid( UPARAM( ref ) FUIExtensionPointHandle& Handle );
};