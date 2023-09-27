// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontedStateComponent.h"
#include "CommonActivatableWidget.h"
#include "NativeGameplayTags.h"
#include "PrimaryGameLayout.h"

namespace FrontendTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PLATFORM_TRAIT_SINGLEONLINEUSER, "Platform.Trait.SingleOnlineUser");
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_GAME, "UI.Layer.Game");
}



UFrontedStateComponent::UFrontedStateComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bShouldShowLoadingScreen = true;
}

void UFrontedStateComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UFrontedStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UFrontedStateComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (bShouldShowLoadingScreen)
	{
		OutReason = TEXT("Loading Layer...");
		return true;
	}
	return false;
}

void UFrontedStateComponent::BeginPlay()
{
	Super::BeginPlay();
	ShowUILayout();
}

void UFrontedStateComponent::ShowUILayout()
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;

		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, HUDLayoutClass,
			[this, RootLayout](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncWidgetLayerState::AfterPush:

				break;
			case EAsyncWidgetLayerState::Canceled:
				break;
			}
			bShouldShowLoadingScreen = false;
		});
	}
}
