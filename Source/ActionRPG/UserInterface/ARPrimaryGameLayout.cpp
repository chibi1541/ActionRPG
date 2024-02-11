// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARPrimaryGameLayout.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPrimaryGameLayout)

UARPrimaryGameLayout::UARPrimaryGameLayout( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{

}

void UARPrimaryGameLayout::RegisterLayer( FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget )
{
	Layers.Add( LayerTag, LayerWidget );
}

UCommonActivatableWidgetContainerBase* UARPrimaryGameLayout::GetLayerWidget( const FGameplayTag& LayerTag ) const
{
	return Layers.FindRef( LayerTag );
}

void UARPrimaryGameLayout::RemoveWidgetFromPrimaryLayer( UCommonActivatableWidget* ActivatableWidget )
{
	for( const auto& LayerKVP : Layers )
	{
		LayerKVP.Value->RemoveWidget( *ActivatableWidget );
	}
}