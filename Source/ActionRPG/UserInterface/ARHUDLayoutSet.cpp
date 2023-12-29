// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARHUDLayoutSet.h"

#include "UserInterface/ARPrimaryGameLayout.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARHUDLayoutSet)

UARHUDLayoutSet::UARHUDLayoutSet()
{

}

UCommonActivatableWidget* UARHUDLayoutSet::AddLayout( UARPrimaryGameLayout* PrimaryLayout )
{
	if( !PrimaryLayout )
	{
		// show error log
		return nullptr;
	}

	return PrimaryLayout->PushWidgetToLayerStack( Layout[0].LayerTag, Layout[0].LayoutClass.Get() );

}

