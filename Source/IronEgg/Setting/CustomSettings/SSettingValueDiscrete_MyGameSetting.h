// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValueDiscrete.h"
#include "SSettingValueDiscrete_MyGameSetting.generated.h"

/**
 * 
 */
UCLASS()
class IRONEGG_API USSettingValueDiscrete_MyGameSetting : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
public:
	USSettingValueDiscrete_MyGameSetting(){};
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

protected:
	virtual void OnInitialized() override;
	virtual void OnApply() override;

public:
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;
protected:
	TArray<FString> SomeNames;

};
