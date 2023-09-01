// Fill out your copyright notice in the Description page of Project Settings.


#include "SAssetManager.h"
#include "SGameplayTags.h"
#include "SLogChannels.h"

USAssetManager::USAssetManager()
{

}

USAssetManager& USAssetManager::Get()
{
	if (USAssetManager* Singleton = Cast<USAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogEgg, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be USAssetManager"));
		return *NewObject<USAssetManager>(); // never calls this
	}
}

void USAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	SGameplayTags::InitializeNativeTags();
}
