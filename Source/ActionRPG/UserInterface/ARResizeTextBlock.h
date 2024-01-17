// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Components/TextBlock.h"

#include "ARResizeTextBlock.generated.h"

/**
 *
 */
UCLASS( meta = ( DisplayName = "ResizeText" ) )
class ACTIONRPG_API UARResizeTextBlock : public UTextBlock
{

	GENERATED_UCLASS_BODY()

public:

	virtual void SetText( FText InText ) override;

protected:

	virtual void SynchronizeProperties() override;

	virtual void OnBindingChanged( const FName& Property ) override;

	virtual void Resize();



};
