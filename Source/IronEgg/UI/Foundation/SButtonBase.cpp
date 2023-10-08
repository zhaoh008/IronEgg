// Fill out your copyright notice in the Description page of Project Settings.


#include "SButtonBase.h"
#include "CommonActionWidget.h"

void USButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void USButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonStyle();
	RefreshButtonText();
}

void USButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();
	UpdateButtonStyle();
	RefreshButtonText();
}

void USButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	UpdateButtonStyle();
}

void USButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}
