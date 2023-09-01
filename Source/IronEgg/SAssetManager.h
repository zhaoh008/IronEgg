// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SAssetManager.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API USAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	USAssetManager();

	static USAssetManager& Get();

	void StartInitialLoading() override;


};
