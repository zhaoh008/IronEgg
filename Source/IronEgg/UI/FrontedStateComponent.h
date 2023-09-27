// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadingProcessInterface.h"
#include "Components/GameStateComponent.h"
#include "FrontedStateComponent.generated.h"

class UCommonActivatableWidget;
/**
 *
 */
UCLASS(Abstract)
class IRONEGG_API UFrontedStateComponent : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UFrontedStateComponent(const FObjectInitializer& ObjectInitializer);


	virtual void InitializeComponent() override;


	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	virtual  bool ShouldShowLoadingScreen(FString& OutReason) const override;




	virtual  void BeginPlay()  override;

private:

	void ShowUILayout();



	UPROPERTY(EditAnywhere, Category = UI)
		TSoftClassPtr<UCommonActivatableWidget> MainScreenClass;

	UPROPERTY(EditAnywhere, Category = UI)
		TSoftClassPtr<UCommonActivatableWidget> HUDLayoutClass;
	bool bShouldShowLoadingScreen;
};
