// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/UserInterface/ARGameUIManagerSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARGameUIManagerSubsystem)



TSubclassOf<UARPrimaryGameLayout> UARGameUIManagerSubsystem::GetLayoutWidgetClass()
{
	return LayoutClass.LoadSynchronous();
}
