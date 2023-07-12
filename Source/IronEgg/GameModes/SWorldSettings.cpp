// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldSettings.h"
#include "GameFramework/PlayerStart.h"
#include "Misc/UObjectToken.h"
#include "Engine/AssetManager.h"
#include "../SLogChannels.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SWorldSettings)

ASWorldSettings::ASWorldSettings(const FObjectInitializer& ObjectInitializer)
{

}
#if WITH_EDITOR
void ASWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	for (TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if (IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapCheck.Warning()
				->AddToken(FUObjectToken::Create(PlayerStart))
				->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, replace with ALyraPlayerStart.")));
		}
	}

}
#endif

FPrimaryAssetId ASWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if (!Result.IsValid())
		{
			UE_LOG(LogEggExperience, Error, TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	return Result;
}
