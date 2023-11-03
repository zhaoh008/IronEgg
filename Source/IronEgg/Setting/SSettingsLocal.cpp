// Fill out your copyright notice in the Description page of Project Settings.


#include "SSettingsLocal.h"

USSettingsLocal* USSettingsLocal::Get()
{
	return GEngine?CastChecked<USSettingsLocal>(GEngine->GetGameUserSettings()):nullptr;
}
