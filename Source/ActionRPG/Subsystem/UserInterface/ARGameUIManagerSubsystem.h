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
	UObject* Widget = nullptr;
};

DECLARE_DELEGATE_TwoParams( FExtendExtensionPointDelegate, EExtensionAction Action, const FUIExtensionRequest& Request );


struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{

public:

	FGameplayTag ExtensionPointTag;
	EExtensionPointMatch TagMatchType = EExtensionPointMatch::ExactMatch;
	TArray<UClass*> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	bool DoesExtensionPassContract( const FUIExtension* Extension ) const;
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
		TObjectPtr<UObject> Widget = nullptr;
};

UCLASS()
class ACTIONRPG_API UARGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	UARGameUIManagerSubsystem();

	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

	TObjectPtr<UARPrimaryGameLayout> GetLayoutWidgetClass();

	void CreatePrimaryLayoutWidget( const ULocalPlayer* LocalPlayer );

	void AddMainGameLayoutWidget();

	FUIExtensionPointHandle RegisterExtensionPoint( const FGameplayTag& ExtensionPointTag, EExtensionPointMatch TagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback );

	FUIExtensionHandle RegisterExtensionWidget( const FGameplayTag& ExtensionPointTag, UObject* WidgetClass );


	void UnregisterExtensionPoint( const FGameplayTag& ExtensionPointTag );

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
