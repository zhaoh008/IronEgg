// Fill out your copyright notice in the Description page of Project Settings.


#include "SLocalPlayer.h"

#include "IronEgg/Setting/SSettingsShared.h"
#include "IronEgg/Setting/SSettingsLocal.h"

void USLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
}

void USLocalPlayer::SwitchController(APlayerController* PC)
{
	Super::SwitchController(PC);
}

bool USLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	return Super::SpawnPlayActor(URL, OutError, InWorld);
}

void USLocalPlayer::InitOnlineSession()
{
	Super::InitOnlineSession();
}

USSettingsLocal* USLocalPlayer::GetLocalSettings() const
{
	return  USSettingsLocal::Get();
}

USSettingsShared* USLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		SharedSettings = USSettingsShared::LoadOrCreateSettings(this);
	}

	return SharedSettings;
}
