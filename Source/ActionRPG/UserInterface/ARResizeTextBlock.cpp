// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARResizeTextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARResizeTextBlock)

UARResizeTextBlock::UARResizeTextBlock( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{
}

void UARResizeTextBlock::SetText( FText InText )
{
	Super::SetText( InText );

	Resize();
}

void UARResizeTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	Resize();

}

void UARResizeTextBlock::OnBindingChanged( const FName& Property )
{
	Super::OnBindingChanged( Property );

	Resize();
}

void UARResizeTextBlock::Resize()
{
	SetRenderScale( FVector2D( 1.f, 1.f ) );

	FVector2D LayoutSize = GetCachedGeometry().GetLocalSize();

	RLOG( Warning, TEXT( "%f, %f" ), LayoutSize.X, LayoutSize.Y );

	if( LayoutSize.X == 0.f )
		return;

	ForceLayoutPrepass();

	FVector2D DesizedSize = GetDesiredSize();

	if( DesizedSize.X > LayoutSize.X )
	{
		float ScaleRate = LayoutSize.X / DesizedSize.X;

		SetRenderScale( FVector2D( ScaleRate, ScaleRate ) );
	}

}
