// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayAbility_Jump.h"
#include "Abilities/GameplayAbilityTypes.h"

USGameplayAbility_Jump::USGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
{
}

bool USGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return false;
}

void USGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
}

void USGameplayAbility_Jump::CharacterJumpStart()
{
}

void USGameplayAbility_Jump::CharacterJumpStop()
{
}
