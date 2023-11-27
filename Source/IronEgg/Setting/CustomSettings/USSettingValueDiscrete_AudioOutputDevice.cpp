// Fill out your copyright notice in the Description page of Project Settings.


#include "USSettingValueDiscrete_AudioOutputDevice.h"

void UUSSettingValueDiscrete_AudioOutputDevice::StoreInitial()
{
	Super::StoreInitial();
}

void UUSSettingValueDiscrete_AudioOutputDevice::ResetToDefault()
{
	Super::ResetToDefault();
}

void UUSSettingValueDiscrete_AudioOutputDevice::RestoreToInitial()
{
	Super::RestoreToInitial();
}

void UUSSettingValueDiscrete_AudioOutputDevice::SetDiscreteOptionByIndex(int32 Index)
{
	Super::SetDiscreteOptionByIndex(Index);
}

int32 UUSSettingValueDiscrete_AudioOutputDevice::GetDiscreteOptionIndex() const
{
	return Super::GetDiscreteOptionIndex();
}

int32 UUSSettingValueDiscrete_AudioOutputDevice::GetDiscreteOptionDefaultIndex() const
{
	return Super::GetDiscreteOptionDefaultIndex();
}

TArray<FText> UUSSettingValueDiscrete_AudioOutputDevice::GetDiscreteOptions() const
{
	return Super::GetDiscreteOptions();
}

void UUSSettingValueDiscrete_AudioOutputDevice::OnInitialized()
{
	Super::OnInitialized();
}
