// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SInputConfig.generated.h"

class UInputAction;


USTRUCT(BlueprintType)
struct FSInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;
};


UCLASS(BlueprintType, Const)
class IRONEGG_API USInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	USInputConfig(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FSInputAction> InputActions;

	UFUNCTION(BlueprintCallable, Category = "Input | Pawn")
		const UInputAction* GetInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;


};
