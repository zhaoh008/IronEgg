// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "SExperienceDefinition.h"
#include "SWorldSettings.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API ASWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	ASWorldSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

	FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
		TSoftClassPtr<USExperienceDefinition> DefaultGameplayExperience;

#if WITH_EDITORONLY_DATA
	// Is this level part of a front-end or other standalone experience?
	// When set, the net mode will be forced to Standalone when you hit Play in the editor
	UPROPERTY(EditDefaultsOnly, Category = PIE)
		bool ForceStandaloneNetMode = false;
#endif
};
