// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/ARExtensionPointWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ARExtensionPointWidget)

#define LOCTEXT_NAMESPACE "UIExtension"

UARExtensionPointWidget::UARExtensionPointWidget( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{

}

void UARExtensionPointWidget::ReleaseSlateResources( bool bReleaseChildren )
{
	ResetExtensionPoint();

	Super::ReleaseSlateResources( bReleaseChildren );
}

TSharedRef<SWidget> UARExtensionPointWidget::RebuildWidget()
{
	if( !IsDesignTime() && ExtensionPointTag.IsValid() )
	{
		ResetExtensionPoint();
		RegisterExtensionPoint();
	}

	// Editor에서 적용되는 코드
	// 다른 프로젝트에서도 효용성이 높아 보임
	if( IsDesignTime() )
	{
		auto GetExtensionPointText = [this]()
		{
			return FText::Format( LOCTEXT( "DesignTime_ExtensionPointLabel", "Extension Point\n{0}" ), FText::FromName( ExtensionPointTag.GetTagName() ) );
		};

		TSharedRef<SOverlay> MessageBox = SNew( SOverlay );

		MessageBox->AddSlot()
			.Padding( 5.0f )
			.HAlign( HAlign_Center )
			.VAlign( VAlign_Center )
			[
				SNew( STextBlock )
				.Justification( ETextJustify::Center )
			.Text_Lambda( GetExtensionPointText )
			];

		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}

void UARExtensionPointWidget::ResetExtensionPoint()
{
	ResetInternal();

	ExtensionMapping.Reset();
	if( ExtensionPointHandle.IsValid() )
		ExtensionPointHandle.Unregister();

	// Check After
	ExtensionPointHandle = FUIExtensionPointHandle();
}

void UARExtensionPointWidget::RegisterExtensionPoint()
{
	if( UARGameUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UARGameUIManagerSubsystem>() )
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add( UUserWidget::StaticClass() );
		AllowedDataClasses.Append( DataClasses );

		ExtensionPointHandle = UIManager->RegisterExtensionPoint(
			ExtensionPointTag, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject( this, &ThisClass::OnAddOrRemoveExtension )
		);
	}
}

void UARExtensionPointWidget::OnAddOrRemoveExtension( EExtensionAction Action, const FUIExtensionRequest& Request )
{
	if( Action == EExtensionAction::Added )
	{
		UObject* Data = Request.Widget;

		TSubclassOf<UUserWidget> WidgetClass( Cast<UClass>( Data ) );
		if( WidgetClass )
		{
			UUserWidget* Widget = CreateEntryInternal( WidgetClass );
			ExtensionMapping.Add( Request.ExtensionHandle, Widget );
		}
		else if( DataClasses.Num() > 0 )
		{
			if( GetWidgetClassForData.IsBound() )
			{
				WidgetClass = GetWidgetClassForData.Execute( Data );

				if( WidgetClass )
				{	
					if( UUserWidget* Widget = CreateEntryInternal( WidgetClass ) )
					{
						ExtensionMapping.Add( Request.ExtensionHandle, Widget );
						ConfigureWidgetForData.ExecuteIfBound( Widget, Data );
					}
				}
			}
		}
	}
	else
	{
		if( UUserWidget* Widget = ExtensionMapping.FindRef( Request.ExtensionHandle ) )
		{
			RemoveEntryInternal( Widget );
			ExtensionMapping.Remove( Request.ExtensionHandle );
		}
	}
}
