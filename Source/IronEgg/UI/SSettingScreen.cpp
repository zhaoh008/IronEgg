// Copyright Epic Games, Inc. All Rights Reserved.

#include "SSettingScreen.h"

#include "Containers/Array.h"
#include "Delegates/Delegate.h"
#include "Input/CommonUIInputTypes.h"
#include "IronEgg/Setting/SGameSettingRegistry.h"
#include "IronEgg/UI/Common/LyraTabListWidgetBase.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SSettingScreen)

class UGameSettingRegistry;

void USSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* USSettingScreen::CreateRegistry()
{
	USGameSettingRegistry* NewRegistry = NewObject<USGameSettingRegistry>();

	if (USLocalPlayer* LocalPlayer = CastChecked<USLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void USSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void USSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void USSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void USSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
