// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"

#include "IronEgg/UI/FrontedStateComponent.h"

ASGameState::ASGameState(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	FrontedStateComponent=CreateDefaultSubobject<UFrontedStateComponent>(TEXT("FrontedStateUI"));
}


void ASGameState::BeginPlay()
{
	Super::BeginPlay();
}

