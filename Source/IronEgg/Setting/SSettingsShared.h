// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UObject/Object.h"
#include "SSettingsShared.generated.h"

class USLocalPlayer;
/**
 *
 */
UCLASS()
class IRONEGG_API USSettingsShared : public USaveGame
{
	GENERATED_BODY()

public:
	USSettingsShared();
	void Initialize(USLocalPlayer* LocalPlayer);
	bool IsDirty() const { return bIsDirty; }
	void ClearDirtyFlag() { bIsDirty = false; }

	void SaveSettings();
	void ApplySettings();
	static USSettingsShared* LoadOrCreateSettings(const USLocalPlayer* LocalPlayer);


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

	FString CurrentName;
};

