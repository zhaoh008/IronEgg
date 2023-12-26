// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SPlayerController.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API ASPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	ASPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void OnRep_PlayerState() override;
};
