// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UObject/Object.h"
#include "SActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ESWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};


/**
 *
 */
UCLASS()
class IRONEGG_API USActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	USActivatableWidget(const FObjectInitializer& ObjectInitializer);


	virtual  TOptional<FUIInputConfig> GetDesiredInputConfig() const override;


#if WITH_EDITOR
	virtual  void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
protected:

	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
		ESWidgetInputMode InputConfig = ESWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
		EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
