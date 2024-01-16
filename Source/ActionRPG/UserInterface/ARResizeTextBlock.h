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
	float GetMaxWidth() const
	{
		return MaxWidth;
	};

	UFUNCTION( BlueprintCallable, Category = "MaxWidth" )
		void SetMaxWidth( float InWidth );


	virtual void SetText( FText InText ) override;

protected:


	virtual void SynchronizeProperties() override;

	virtual void OnBindingChanged( const FName& Property ) override;

	virtual void Resize();

protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetMaxWidth", Category = "MaxWidth", meta = ( ClampMin = 0.0 ) )
		float MaxWidth;

};
