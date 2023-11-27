// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValueDiscreteDynamic.h"
#include "UObject/Object.h"
#include "USSettingValueDiscrete_AudioOutputDevice.generated.h"

/**
 * 
 */
UCLASS()
class IRONEGG_API UUSSettingValueDiscrete_AudioOutputDevice : public UGameSettingValueDiscreteDynamic
{
	GENERATED_BODY()
public:
	UUSSettingValueDiscrete_AudioOutputDevice(){};
	
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual int32 GetDiscreteOptionDefaultIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	virtual void OnInitialized() override;
};
