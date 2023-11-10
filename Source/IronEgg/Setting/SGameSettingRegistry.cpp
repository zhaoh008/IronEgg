// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingRegistry.h"
#include "GameSettingCollection.h"
#include "IronEgg/Player/SLocalPlayer.h"
#include "IronEgg/Setting/SSettingsLocal.h"
#include "IronEgg/Setting/SSettingsShared.h"

USGameSettingRegistry* USGameSettingRegistry::Get(USLocalPlayer* LocalPlayer)
{
	USGameSettingRegistry* Registry = FindObject<USGameSettingRegistry>(LocalPlayer, TEXT("SGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<USGameSettingRegistry>(LocalPlayer, TEXT("SGameSettingRegistry"));
		Registry -> Initialize(LocalPlayer);
	}

	return Registry;
}

void USGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	Super::SaveChanges();
	
	if (USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

void USGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(InLocalPlayer);

	GameplaySettings = InitializeGameplaySettings(LocalPlayer);
	RegisterSetting(GameplaySettings);
	
	MyGameSetting=InitializeMyGameSettingCollection(LocalPlayer);
	RegisterSetting(MyGameSetting);

}

bool USGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

