// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/HitTraceComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitTraceComponent)

UHitTraceComponent::UHitTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bHitSameSocketAtDifferentTimes = true;
	bHitOtherSocketsAtSameTime = false;
	bHitOtherSocketsAtDifferentTime = false;

	CanTrace = false;

	QueryType = ETraceQueryType::ChannelQuery;
	ShapeType = ETraceShapeType::LineTrace;
	ChannelQueryType = UEngineTypes::ConvertToTraceType( ECC_Visibility );
	ObjectQueryType = { UEngineTypes::ConvertToObjectType( ECC_WorldStatic ), UEngineTypes::ConvertToObjectType( ECC_WorldDynamic ) };
	ShouldTraceComplex = true;
	ShouldIgnoreSelf = true;

	DrawTime = 0.f;
	TraceColor = FLinearColor::Red;
	TraceHitColor = FLinearColor::Green;
	DrawDebugType = EDrawDebugTrace::Type::ForDuration;
}


void UHitTraceComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( CanTrace )
	{
		if( bHitSameSocketAtDifferentTimes )
		{
			HitSameSocketAtDifferentTimes();
		}

		if( bHitOtherSocketsAtSameTime )
		{
			HitOtherSocketsAtSameTime();
		}

		if( bHitOtherSocketsAtDifferentTime )
		{
			HitOtherSocketsAtDifferentTime();
		}

		UpdateLastSocketLocation();
	}
}

void UHitTraceComponent::HitSameSocketAtDifferentTimes()
{
	for( const auto SocketName : SocketNames )
	{
		const FVector* Start = LastSocketLocations.Find( SocketName );
		const FVector End = PrimitiveComp->GetSocketLocation( SocketName );

		if( Start == nullptr )
		{
			return;
		}

		switch( QueryType )
		{
			case ETraceQueryType::ChannelQuery:
			{
				TracebyChannelType( *Start, End );
				break;
			}
			case ETraceQueryType::ObjectQuery:
			{
				TracebyObjectType( *Start, End );
				break;
			}
		}
	}
}

void UHitTraceComponent::HitOtherSocketsAtSameTime()
{
	int iIndex = 0;
	int jIndex = 0;

	for( iIndex; iIndex < ( SocketNames.Num() - 1 ) ; ++iIndex )
	{
		iIndex = jIndex + 1;
		for( jIndex; jIndex < SocketNames.Num() ; ++jIndex )
		{
			FVector Start = PrimitiveComp->GetSocketLocation( SocketNames[iIndex] );
			FVector End = PrimitiveComp->GetSocketLocation( SocketNames[jIndex] );

			switch( QueryType )
			{
				case ETraceQueryType::ChannelQuery:
				{
					TracebyChannelType( Start, End );
					break;
				}
				case ETraceQueryType::ObjectQuery:
				{
					TracebyObjectType( Start, End );
					break;
				}
			}
		}
	}
}

void UHitTraceComponent::HitOtherSocketsAtDifferentTime()
{
	for( const auto SocketName_0 : SocketNames )
	{
		for( const auto SocketName_2 : SocketNames )
		{
			TArray<FHitResult> OutHits1;
			FVector Start = PrimitiveComp->GetSocketLocation( SocketName_0 );
			FVector* End = LastSocketLocations.Find( SocketName_2 );

			if( End == nullptr )
			{
				return;
			}

			switch( QueryType )
			{
				case ETraceQueryType::ChannelQuery:
				{
					TracebyChannelType( Start, *End );
					break;
				}
				case ETraceQueryType::ObjectQuery:
				{
					TracebyObjectType( Start, *End );
					break;
				}
			}
		}
	}
}

void UHitTraceComponent::TracebyChannelType( const FVector Start, const FVector End )
{
	TArray<FHitResult> OutHits;

	AActor* Actor = GetOwner();

	switch( ShapeType )
	{
		case ETraceShapeType::LineTrace:
		{
			UKismetSystemLibrary::LineTraceMulti( Actor, Start, End, ChannelQueryType, ShouldTraceComplex, IgnoreActors,
				DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}

		case ETraceShapeType::BoxTrace:
		{
			UKismetSystemLibrary::BoxTraceMulti( Actor, Start, End, BoxHalfSize, BoxOrientation, ChannelQueryType,
				ShouldTraceComplex, IgnoreActors, DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}

		case ETraceShapeType::SphereTrace:
		{
			UKismetSystemLibrary::SphereTraceMulti( Actor, Start, End, SphereRadius, ChannelQueryType,
				ShouldTraceComplex, IgnoreActors, DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}
	}

	AddHitArray( OutHits );
}

void UHitTraceComponent::TracebyObjectType( const FVector Start, const FVector End )
{
	TArray<FHitResult> OutHits;

	AActor* Actor = GetOwner();

	switch( ShapeType )
	{
		case ETraceShapeType::LineTrace:
		{
			UKismetSystemLibrary::LineTraceMultiForObjects( Actor, Start, End, ObjectQueryType, ShouldTraceComplex, IgnoreActors,
				DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}

		case ETraceShapeType::BoxTrace:
		{
			UKismetSystemLibrary::BoxTraceMultiForObjects( Actor, Start, End, BoxHalfSize, BoxOrientation, ObjectQueryType,
				ShouldTraceComplex, IgnoreActors, DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}

		case ETraceShapeType::SphereTrace:
		{
			UKismetSystemLibrary::SphereTraceMultiForObjects( Actor, Start, End, SphereRadius, ObjectQueryType,
				ShouldTraceComplex, IgnoreActors, DrawDebugType, OutHits, ShouldIgnoreSelf, TraceColor, TraceHitColor, DrawTime );
			break;
		}
	}

	AddHitArray( OutHits );
}

void UHitTraceComponent::AddHitArray( const TArray<FHitResult> HitResults )
{
	for( const auto& HitResult : HitResults )
	{
		if( !HitArray.ContainsByPredicate( [&] ( const FHitResult& Inner ) { return Inner.GetActor() == HitResult.GetActor(); } ) )
		{
			HitArray.Add( HitResult );

			OnHit.Broadcast( HitResult );
		}
	}
}

void UHitTraceComponent::UpdateLastSocketLocation()
{
	if( SocketNames.Num() > 0 )
	{

		for( auto SocketName : SocketNames )
		{
			LastSocketLocations.Add( SocketName, PrimitiveComp->GetSocketLocation( SocketName ) );
		}
	}
}

void UHitTraceComponent::SetSockets( const TArray<FName> TargetSockets )
{
	SocketNames.Append( TargetSockets );
}

void UHitTraceComponent::ClearSockets()
{
	SocketNames.Empty();
}

void UHitTraceComponent::ToggleTraceCheck( bool CanTraceInput )
{
	if( CanTraceInput )
	{
		InitData();
	}

	CanTrace = CanTraceInput;
}

void UHitTraceComponent::InitData()
{
	HitArray.Empty();
	LastSocketLocations.Empty();
	for( auto Socket : SocketNames )
	{
		LastSocketLocations.Add( Socket, PrimitiveComp->GetSocketLocation( Socket ) );
	}
}

bool UHitTraceComponent::SetupBaseData( UPrimitiveComponent* PrimitiveAsset )
{
	if( PrimitiveAsset != nullptr )
	{
		PrimitiveComp = PrimitiveAsset;

		return true;
	}

	return false;
}