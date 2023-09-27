// Fill out your copyright notice in the Description page of Project Settings.


#include "SActivatableWidget.h"
#include "Editor/WidgetCompilerLog.h"

#define LOCTEXT_NAMESPACE "IronEgg"

USActivatableWidget::USActivatableWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> USActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{

	case ESWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case ESWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case ESWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case ESWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

void USActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(USActivatableWidget, BP_GetDesiredFocusTarget)))
	{
		if (GetParentNativeClass(GetClass()) == USActivatableWidget::StaticClass())
		{
			CompileLog.Warning(LOCTEXT("ValidateGetDesiredFocusTarget_Warning", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."));
		}
		else
		{
			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
			CompileLog.Note(LOCTEXT("ValidateGetDesiredFocusTarget_Note", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message."));
		}
	}
}
