// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UObject/Object.h"
#include "SSettingsLocal.generated.h"

/**
 * 
 */
UCLASS()
class IRONEGG_API USSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:
	USSettingsLocal(){};
	
	static USSettingsLocal* Get();
	
};
