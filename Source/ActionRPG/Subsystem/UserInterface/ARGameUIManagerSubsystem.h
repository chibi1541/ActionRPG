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
	int32 Priority = INDEX_NONE;
	TWeakObjectPtr<UObject> ContextObject;
	UObject* Data = nullptr;
};

DECLARE_DELEGATE_TwoParams( FExtendExtensionPointDelegate, EExtensionAction Action, const FUIExtensionRequest& Request );


struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{

public:

	FGameplayTag ExtensionPointTag;
	TWeakObjectPtr<UObject> ContextObject;
	EExtensionPointMatch TagMatchType = EExtensionPointMatch::ExactMatch;
	TArray<UClass*> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	bool DoesExtensionPassContract( const FUIExtension* Extension ) const;
};

USTRUCT( BlueprintType )
struct ACTIONRPG_API FUIExtensionPointHandle
{
	GENERATED_BODY()

public:
	FUIExtensionPointHandle() {}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==( const FUIExtensionPointHandle& Other ) const { return DataPtr == Other.DataPtr; }
	bool operator!=( const FUIExtensionPointHandle& Other ) const { return !operator==( Other ); }

	friend FORCEINLINE uint32 GetTypeHash( const FUIExtensionPointHandle& Handle )
	{
		return PointerHash( Handle.DataPtr.Get() );
	}

private:
	TSharedPtr<FUIExtensionPoint> DataPtr;

	friend UARGameUIManagerSubsystem;

	FUIExtensionPointHandle( const TSharedPtr<FUIExtensionPoint>& InDataPtr ) : DataPtr( InDataPtr ) {}
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
	TSharedPtr<FUIExtension> DataPtr;

	friend UARGameUIManagerSubsystem;

	FUIExtensionHandle( const TSharedPtr<FUIExtension>& InDataPtr ) : DataPtr( InDataPtr ) {}
};

USTRUCT( BlueprintType )
struct FUIExtensionRequest
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		FUIExtensionHandle ExtensionHandle;

	// Widget Class
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

	TObjectPtr<UARPrimaryGameLayout> GetLayoutWidgetClass();

	void CreatePrimaryLayoutWidget( const ULocalPlayer* LocalPlayer );

	void AddMainGameLayoutWidget();

	FUIExtensionPointHandle RegisterExtensionPoint( const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EExtensionPointMatch TagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback );


protected:
	void NotifyExtensionPointOfExtensions( TSharedPtr<FUIExtensionPoint>& ExtensionPoint );

private:
	UPROPERTY( Transient )
		TObjectPtr<UARPrimaryGameLayout> PrimaryLayout;

	TArray<TWeakObjectPtr<UCommonActivatableWidget>> Layouts;


	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;

	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
};
