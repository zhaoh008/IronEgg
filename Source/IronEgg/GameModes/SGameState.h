﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "SGameState.generated.h"

class UFrontedStateComponent;


/**
 *
 */
UCLASS()
class IRONEGG_API ASGameState : public AModularGameState
{
	GENERATED_BODY()
public:
	ASGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual  void BeginPlay() override;
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "GameState")
	UFrontedStateComponent * FrontedStateComponent;


	
};
