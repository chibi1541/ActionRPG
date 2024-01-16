// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARResizeTextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARResizeTextBlock)

UARResizeTextBlock::UARResizeTextBlock( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void UARResizeTextBlock::SetMaxWidth( float InWidth )
{
	MaxWidth = InWidth;
}

void UARResizeTextBlock::SetText( FText InText )
{
	Super::SetText( InText );

	RLOG_L( Warning );

	Resize();
}

void UARResizeTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	RLOG_L( Warning );

	Resize();

}

void UARResizeTextBlock::OnBindingChanged( const FName& Property )
{
	Super::OnBindingChanged( Property );

	RLOG_L( Warning );

	Resize();
}

void UARResizeTextBlock::Resize()
{
	SetRenderScale( FVector2D( 1.f, 1.f ) );

	if( MaxWidth == 0.f )
		return;

	ForceLayoutPrepass();

	FVector2D DesizedSize = GetDesiredSize();

	FVector2D LayoutSize = GetCachedGeometry().GetLocalSize();

	RLOG(Warning, TEXT("Layout Size : %f, %f"), LayoutSize.X, LayoutSize.Y );

	if( DesizedSize.X > MaxWidth )
	{
		float ScaleRate = MaxWidth / DesizedSize.X;

		SetRenderScale( FVector2D( ScaleRate, ScaleRate ) );
	}

}
