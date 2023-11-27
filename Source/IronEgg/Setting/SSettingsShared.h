// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SSettingsShared.generated.h"

class USLocalPlayer;


UENUM(BlueprintType)
enum class ELyraAllowBackgroundAudioSetting : uint8
{
	Off,
	AllSounds,

	Num UMETA(Hidden),
};
/**
 *
 */
UCLASS()
class IRONEGG_API USSettingsShared : public USaveGame
{
	GENERATED_BODY()
	
public:
	USSettingsShared();
	
	DECLARE_EVENT_OneParam(USSettingsShared, FOnSettingChangedEvent, USSettingsShared* Settings);
	FOnSettingChangedEvent OnSettingChanged;
	
	void Initialize(USLocalPlayer* LocalPlayer);
	bool IsDirty() const { return bIsDirty; }
	void ClearDirtyFlag() { bIsDirty = false; }

	void SaveSettings();
	void ApplySettings();
	static USSettingsShared* LoadOrCreateSettings(const USLocalPlayer* LocalPlayer);

	
	////////////////////////////////////////////////////////
	/// Dirty and Change Reporting
private:
	template<typename T>
	bool ChangeValueAndDirty(T& CurrentValue, const T& NewValue)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			bIsDirty = true;
			OnSettingChanged.Broadcast(this);
			
			return true;
		}

		return false;
	}

private:
	UPROPERTY(Transient)
		TObjectPtr<USLocalPlayer> OwningPlayer = nullptr;

	bool bIsDirty = false;
	////////////////////////////////////////////////////////
	// Culture / language
public:
	/** Gets the pending culture */
	const FString& GetPendingCulture() const;

	/** Sets the pending culture to apply */
	void SetPendingCulture(const FString& NewCulture);

	// Called when the culture changes.
	void OnCultureChanged();

	/** Clears the pending culture to apply */
	void ClearPendingCulture();

	bool IsUsingDefaultCulture() const;

	void ResetToDefaultCulture();
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }

	void ApplyCultureSettings();
	void ResetCultureToCurrentSettings();




private:
	/** The pending culture to apply */
	UPROPERTY(Transient)
		FString PendingCulture;

	/* If true, resets the culture to default. */
	bool bResetToDefaultCulture = false;

	////////////////////////////////////////////////////////
	// Gamepad Sensitivity


	/////我的游戏设置

public:

	TArray<FString> GetSomeNameList() const;

	void SetCurrentName(const FString& InName);

	FString GetCurrentName()const {
		return CurrentName;
	};

	void ApplyMyGameSetting();


	void ClearCurrentName();

	int32 FindCurrentNameIndex() const;

private:


	TArray<FString> SomeNames;

	UPROPERTY()
		FString CurrentName;

	////////////////////////////////////////////////////////
	// Shared audio settings
	public:
	UFUNCTION()
	ELyraAllowBackgroundAudioSetting GetAllowAudioInBackgroundSetting() const { return AllowAudioInBackground; }
	UFUNCTION()
	void SetAllowAudioInBackgroundSetting(ELyraAllowBackgroundAudioSetting NewValue);

	void ApplyBackgroundAudioSettings();

private:
	UPROPERTY()
	ELyraAllowBackgroundAudioSetting AllowAudioInBackground = ELyraAllowBackgroundAudioSetting::Off;
};

