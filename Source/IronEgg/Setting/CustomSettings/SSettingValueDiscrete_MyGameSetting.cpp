// Fill out your copyright notice in the Description page of Project Settings.


#include "SSettingValueDiscrete_MyGameSetting.h"
#include "../SSettingsShared.h"
#include "IronEgg/Player/SLocalPlayer.h"

#define LOCTEXT_NAMESPACE "IronEgg"

void USSettingValueDiscrete_MyGameSetting::StoreInitial()
{

}

void USSettingValueDiscrete_MyGameSetting::ResetToDefault()
{
	SetDiscreteOptionByIndex(0);
}

void USSettingValueDiscrete_MyGameSetting::RestoreToInitial()
{
	if (USSettingsShared* Setting = CastChecked<USLocalPlayer>(LocalPlayer)->GetSharedSettings()) {
		Setting->ClearCurrentName();
		NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
	}
}

void USSettingValueDiscrete_MyGameSetting::OnInitialized()
{
	if (const USSettingsShared* Setting = CastChecked<USLocalPlayer>(LocalPlayer)->GetSharedSettings()) {
		SomeNames = Setting->GetSomeNameList();
	}
}

void USSettingValueDiscrete_MyGameSetting::OnApply()
{
	Super::OnApply();
}

void USSettingValueDiscrete_MyGameSetting::SetDiscreteOptionByIndex(int32 Index)
{
	if (USSettingsShared* Setting = CastChecked<USLocalPlayer>(LocalPlayer)->GetSharedSettings()) {
		if (SomeNames.IsValidIndex(Index))
		{
			Setting->SetCurrentName(SomeNames[Index]);
		}
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 USSettingValueDiscrete_MyGameSetting::GetDiscreteOptionIndex() const
{
	if (USSettingsShared* Setting = CastChecked<USLocalPlayer>(LocalPlayer)->GetSharedSettings()) {
		return  Setting->FindCurrentNameIndex();
	}
	return 0;
}

TArray<FText> USSettingValueDiscrete_MyGameSetting::GetDiscreteOptions() const
{
	TArray<FText> Options;
	for (FString Name : SomeNames)
	{
		FText LocalizedSystemDefault = FText::Format(LOCTEXT("选项", "({0})"), FText::FromString(Name));
		Options.Add(LocalizedSystemDefault);
	}

	return Options;
}
#undef LOCTEXT_NAMESPACE