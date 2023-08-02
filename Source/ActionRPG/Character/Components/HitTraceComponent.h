// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "HitTraceComponent.generated.h"

UENUM( BlueprintType )
enum class ETraceShapeType : uint8
{
	LineTrace		UMETA( DisplayName = "Line Trace" ),
	BoxTrace		UMETA( DiaplayName = "Box Trace" ),
	SphereTrace		UMETA( DisplayName = "Sphere Trace" )
};

UENUM( BlueprintType )
enum class ETraceQueryType : uint8
{
	ChannelQuery		UMETA( DisplayName = "Channel Type" ),
	ObjectQuery			UMETA( DisplayName = "Object Type" )
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHitEvent, FHitResult, HitResult );

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent, DisplayName = "HitTrace" ) )
class ACTIONRPG_API UHitTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitTraceComponent();

#pragma region Public Variables

public:
	UPROPERTY( BlueprintAssignable, Category = "HitTrace" )
		FOnHitEvent OnHit;

#pragma endregion

#pragma region Protectd Variables

protected:
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Debug" )
	TObjectPtr<class UPrimitiveComponent> PrimitiveComp;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Initialize" )
	TArray<FName> SocketNames;

	UPROPERTY()
	TMap<FName, FVector> LastSocketLocations;

#pragma endregion

#pragma region Private Variables
private:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Debug", meta = ( AllowPrivateAccess = "true" ) )
		bool CanTrace;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Debug", meta = ( AllowPrivateAccess = "true" ) )
		TArray<FHitResult> HitArray;



	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TraceMethodType", meta = ( AllowPrivateAccess = "true" ) )
		bool bHitSameSocketAtDifferentTimes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TraceMethodType", meta = ( AllowPrivateAccess = "true" ) )
		bool bHitOtherSocketsAtSameTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TraceMethodType", meta = ( AllowPrivateAccess = "true" ) )
		bool bHitOtherSocketsAtDifferentTime;



	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		ETraceQueryType QueryType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		FVector BoxHalfSize;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		FRotator BoxOrientation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		float SphereRadius;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		ETraceShapeType ShapeType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		TEnumAsByte<ETraceTypeQuery> ChannelQueryType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQueryType = { UEngineTypes::ConvertToObjectType( ECC_WorldStatic ), UEngineTypes::ConvertToObjectType( ECC_WorldDynamic ) };

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		bool ShouldTraceComplex;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		TArray<AActor*> IgnoreActors;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingSetting", meta = ( AllowPrivateAccess = "true" ) )
		bool ShouldIgnoreSelf;



	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingDebug", meta = ( AllowPrivateAccess = "true" ) )
		float DrawTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingDebug", meta = ( AllowPrivateAccess = "true" ) )
		FLinearColor TraceColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingDebug", meta = ( AllowPrivateAccess = "true" ) )
		FLinearColor TraceHitColor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "TracingDebug", meta = ( AllowPrivateAccess = "true" ) )
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

#pragma endregion


protected:
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void HitSameSocketAtDifferentTimes();

	virtual void HitOtherSocketsAtSameTime();

	virtual void HitOtherSocketsAtDifferentTime();

	virtual void TracebyChannelType( const FVector Start, const FVector End );

	virtual void TracebyObjectType( const FVector Start, const FVector End );

	virtual void AddHitArray( const TArray<FHitResult> HitResults );

	virtual void UpdateLastSocketLocation();

	

public:
	UFUNCTION( BlueprintCallable, Category = "HitTrace" )
	bool SetupBaseData( UPrimitiveComponent* PrimitiveAsset );

	UFUNCTION( BlueprintCallable, Category = "HitTrace" )
	virtual void ToggleTraceCheck( bool CanTraceInput );

	UFUNCTION( BlueprintCallable, Category = "HitTrace" )
	virtual void InitData();
	
	virtual void SetSockets( const TArray<FName> TargetSockets );

	virtual void ClearSockets();

public:
	FORCEINLINE	TArray<FName> GetSockets() const { return SocketNames; }

};
