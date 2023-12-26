// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayAbility.h"

#include "SGameplayAbility_Jump.generated.h"


class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;
/**
 * 
 */
UCLASS()
class IRONEGG_API USGameplayAbility_Jump : public USGameplayAbility
{
	GENERATED_BODY()
public:
	USGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer);


protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	void CharacterJumpStop();
	
};
