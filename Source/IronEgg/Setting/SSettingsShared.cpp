// Fill out your copyright notice in the Description page of Project Settings.


#include "SSettingsShared.h"
#include "Internationalization/Culture.h"
#include "Internationalization/CulturePointer.h"
#include "Internationalization/Internationalization.h"
#include "IronEgg/Player/SLocalPlayer.h"
#include "Kismet/GameplayStatics.h"

static FString SHARED_SETTINGS_SLOT_NAME = TEXT("SharedGameSettings");

void USSettingsShared::Initialize(USLocalPlayer* LocalPlayer)
{
	check(LocalPlayer);
	OwningPlayer=LocalPlayer;
}

void USSettingsShared::SaveSettings()
{
	check(OwningPlayer);
	UGameplayStatics::SaveGameToSlot(this, SHARED_SETTINGS_SLOT_NAME, OwningPlayer->GetLocalPlayerIndex());
}

void USSettingsShared::ApplySettings()
{
	ApplyCultureSettings();
}

USSettingsShared* USSettingsShared::LoadOrCreateSettings(const USLocalPlayer* LocalPlayer)
{
	USSettingsShared* SharedSettings = nullptr;

	// If the save game exists, load it.
	if (UGameplayStatics::DoesSaveGameExist(SHARED_SETTINGS_SLOT_NAME, LocalPlayer->GetLocalPlayerIndex()))
	{
		USaveGame* Slot = UGameplayStatics::LoadGameFromSlot(SHARED_SETTINGS_SLOT_NAME, LocalPlayer->GetLocalPlayerIndex());
		SharedSettings = Cast<USSettingsShared>(Slot);
	}
	
	if (SharedSettings == nullptr)
	{
		SharedSettings = Cast<USSettingsShared>(UGameplayStatics::CreateSaveGameObject(USSettingsShared::StaticClass()));
	}

	SharedSettings->Initialize(const_cast<USLocalPlayer*>(LocalPlayer));
	SharedSettings->ApplySettings();

	return SharedSettings;
}

const FString& USSettingsShared::GetPendingCulture() const
{
	return PendingCulture;
}

void USSettingsShared::SetPendingCulture(const FString& NewCulture)
{
	PendingCulture = NewCulture;
	bResetToDefaultCulture = false;
	bIsDirty = true;
}

void USSettingsShared::OnCultureChanged()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void USSettingsShared::ClearPendingCulture()
{
	PendingCulture.Reset();
}

bool USSettingsShared::IsUsingDefaultCulture() const
{
	FString Culture;
	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);
	return Culture.IsEmpty();
}

void USSettingsShared::ResetToDefaultCulture()
{
	ClearPendingCulture();
	bResetToDefaultCulture = true;
	bIsDirty = true;
}

void USSettingsShared::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		check(SystemDefaultCulture.IsValid());

		const FString CultureToApply = SystemDefaultCulture->GetName();
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Clear this string
			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		// SetCurrentCulture may trigger PendingCulture to be cleared (if a culture change is broadcast) so we take a copy of it to work with
		const FString CultureToApply = PendingCulture;
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Note: This is intentionally saved to the users config
			// We need to localize text before the player logs in and very early in the loading screen
			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		ClearPendingCulture();
	}
}

void USSettingsShared::ResetCultureToCurrentSettings()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}
