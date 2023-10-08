// Fill out your copyright notice in the Description page of Project Settings.


#include "SConfirmationScreen.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#include "CommonBorder.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "SButtonBase.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"


void USConfirmationScreen::SetupDialog(UCommonGameDialogDescriptor* Descriptor,
                                       FCommonMessagingResultDelegate ResultCallback)
{
	Super::SetupDialog(Descriptor, ResultCallback);
	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

	EntryBox_Buttons->Reset<USButtonBase>([](USButtonBase& Button)
	{
		Button.OnClicked().Clear();
	});

	for (const FConfirmationDialogAction& Action : Descriptor->ButtonActions)
	{
		FDataTableRowHandle ActionRow;

		switch(Action.Result)
		{
		case ECommonMessagingResult::Confirmed:
			ActionRow = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;
		case ECommonMessagingResult::Declined:
			ActionRow = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case ECommonMessagingResult::Cancelled:
			ActionRow = CancelAction;
			break;
		default:
			ensure(false);
			continue;
		}

		USButtonBase* Button = EntryBox_Buttons->CreateEntry<USButtonBase>();
		Button->SetTriggeringInputAction(ActionRow);
		Button->OnClicked().AddUObject(this, &ThisClass::CloseConfirmationWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}

	OnResultCallback = ResultCallback;
}

void USConfirmationScreen::KillDialog()
{
	Super::KillDialog();
}

void USConfirmationScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Border_TapToCloseZone->OnMouseButtonDownEvent.BindDynamic(this, &USConfirmationScreen::HandleTapToCloseZoneMouseButtonDown);
}

void USConfirmationScreen::CloseConfirmationWindow(ECommonMessagingResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}

#if WITH_EDITOR
void USConfirmationScreen::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (CancelAction.IsNull())
	{
		CompileLog.Error(FText::Format(FText::FromString(TEXT("{0} has unset property: CancelAction.")), FText::FromString(GetName())));
	}
}
#endif


FEventReply USConfirmationScreen::HandleTapToCloseZoneMouseButtonDown(FGeometry MyGeometry,
                                                                      const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();

	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CloseConfirmationWindow(ECommonMessagingResult::Declined);
		Reply.NativeReply = FReply::Handled();
	}

	return Reply;
}
