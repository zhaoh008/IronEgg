// Fill out your copyright notice in the Description page of Project Settings.


#include "SBotCharacter.h"

// Sets default values
ASBotCharacter::ASBotCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASBotCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASBotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASBotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

