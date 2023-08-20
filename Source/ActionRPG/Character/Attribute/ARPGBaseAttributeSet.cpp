// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ARPGBaseAttributeSet.h"



#include UE_INLINE_GENERATED_CPP_BY_NAME(ARPGBaseAttributeSet)

UARPGBaseAttributeSet::UARPGBaseAttributeSet( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

void UARPGBaseAttributeSet::PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue )
{
	RLOG( Warning, TEXT( "Call PreAttributeChange Execute : %s, %f" ), *( Attribute.AttributeName ), NewValue );
}
