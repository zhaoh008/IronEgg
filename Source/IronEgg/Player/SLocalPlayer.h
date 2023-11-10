// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "SLocalPlayer.generated.h"

class USSettingsShared;
/**
 *
 */
UCLASS()
class IRONEGG_API USLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
public:
	USLocalPlayer() {}

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	UFUNCTION()
		USSettingsLocal* GetLocalSettings() const;

	UFUNCTION()
		USSettingsShared* GetSharedSettings() const;

private:
	UPROPERTY(Transient)
		mutable TObjectPtr<USSettingsShared> SharedSettings;

};
