// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "IronEgg/Player/SLocalPlayer.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "SGameSettingRegistry.generated.h"

class ULocalPlayer;
class UObject;

//--------------------------------------
// ULyraGameSettingRegistry
//--------------------------------------

class UGameSettingCollection;
class USLocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogLyraGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
		MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({							\
		GET_FUNCTION_NAME_STRING_CHECKED(USLocalPlayer, GetSharedSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(USSettingsShared, FunctionOrPropertyName)		\
}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
		MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({							\
		GET_FUNCTION_NAME_STRING_CHECKED(USLocalPlayer, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(USSettingsLocal, FunctionOrPropertyName)		\
}))

/**
 * 
 */
UCLASS()
class IRONEGG_API USGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()
public:
	USGameSettingRegistry(){};


	static  USGameSettingRegistry *Get( USLocalPlayer *LocalPlayer);
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeAudioSettings(USLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeVideoSettings(USLocalPlayer* InLocalPlayer);
	
	UGameSettingCollection*  InitializeGameplaySettings(USLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeMyGameSettingCollection(USLocalPlayer * InLocalPlayer);

	

private:
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GamepadSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MyGameSetting;
};
