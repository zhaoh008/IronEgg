// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "SBotCharacter.generated.h"

UCLASS()
class IRONEGG_API ASBotCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:

	ASBotCharacter();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
