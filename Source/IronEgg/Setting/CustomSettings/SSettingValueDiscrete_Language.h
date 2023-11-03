// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValueDiscrete.h"
#include "UObject/Object.h"
#include "SSettingValueDiscrete_Language.generated.h"

/**
 * 
 */
UCLASS()
class IRONEGG_API USSettingValueDiscrete_Language : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	USSettingValueDiscrete_Language();

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	virtual void OnApply() override;

protected:
	TArray<FString> AvailableCultureNames;

};
