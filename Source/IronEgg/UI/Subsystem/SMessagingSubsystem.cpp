// Fill out your copyright notice in the Description page of Project Settings.


#include "SMessagingSubsystem.h"

#include "NativeGameplayTags.h"
#include "PrimaryGameLayout.h"
#include "IronEgg/Player/SLocalPlayer.h"
#include "Messaging/CommonGameDialog.h"
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MODAL, "UI.Layer.Modal");

void USMessagingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ConfirmationDialogClassPtr=ConfirmationDialogClass.LoadSynchronous();
	ErrorDialogClassPtr=ErrorDialogClass.LoadSynchronous();
}

void USMessagingSubsystem::ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor,
                                            FCommonMessagingResultDelegate ResultCallback)
{
	USLocalPlayer* LocalPlayer=GetLocalPlayer<USLocalPlayer>();
	if (LocalPlayer)
	{
		if (UPrimaryGameLayout *Layout=	LocalPlayer->GetRootUILayout())
		{
			Layout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_UI_LAYER_MODAL,ConfirmationDialogClassPtr,[DialogDescriptor,ResultCallback](UCommonGameDialog &Dialog)
			{
				Dialog.SetupDialog(DialogDescriptor,ResultCallback);
			});
		}
	}
}

void USMessagingSubsystem::ShowError(UCommonGameDialogDescriptor* DialogDescriptor,
	FCommonMessagingResultDelegate ResultCallback)
{
	USLocalPlayer* LocalPlayer=GetLocalPlayer<USLocalPlayer>();
	if (LocalPlayer)
	{
		if (UPrimaryGameLayout *Layout=	LocalPlayer->GetRootUILayout())
		{
			Layout->PushWidgetToLayerStack<UCommonGameDialog>(TAG_UI_LAYER_MODAL,ErrorDialogClassPtr,[DialogDescriptor,ResultCallback](UCommonGameDialog &Dialog)
			{
				Dialog.SetupDialog(DialogDescriptor,ResultCallback);
			});
		}
	}
}
