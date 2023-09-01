// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "SInputConfig.h"
#include "SEnhancedInputComponent.generated.h"

/**
 *
 */
UCLASS(Config = Input)
class IRONEGG_API USEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	USEnhancedInputComponent(const FObjectInitializer& ObjectInitializer);

	template<typename UserClass, typename FunType >
	void BindSAction(const USInputConfig* InputConfig, const FGameplayTag& GamePlayTag, ETriggerEvent TriggerEvent, UserClass* Obj, FunType Fun, bool bLogIfNotFound);
};

template<typename UserClass, typename FunType >
void USEnhancedInputComponent::BindSAction(const USInputConfig* InputConfig, const FGameplayTag& GamePlayTag, ETriggerEvent TriggerEvent, UserClass* Obj, FunType Fun, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->GetInputActionByTag(GamePlayTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Obj, Fun);
	}
}
