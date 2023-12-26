// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

#include "SGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class IRONEGG_API USGameplayAbility : public  UGameplayAbility
{
	GENERATED_BODY()
public:
	USGameplayAbility(const FObjectInitializer& ObjectInitializer=FObjectInitializer::Get());

	
};
