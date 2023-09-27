// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActivatableWidget.h"
#include "UObject/Object.h"
#include "SHUDLayout.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API USHUDLayout : public USActivatableWidget
{
	GENERATED_BODY()

public:
	USHUDLayout(const FObjectInitializer& ObjectInitializer);

	virtual  void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
