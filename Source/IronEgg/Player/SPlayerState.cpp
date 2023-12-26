// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent=CreateDefaultSubobject<USAbilitySystemComponent>(TEXT("AbilityComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet=CreateDefaultSubobject<USAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.0f;
}

USAbilitySystemComponent* ASPlayerState::GetSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* ASPlayerState::GetAbilitySystemComponent() const
{
	return GetSAbilitySystemComponent();
}

USAttributeSet* ASPlayerState::GetSAttributeSet() const
{
	return AttributeSet;
}

