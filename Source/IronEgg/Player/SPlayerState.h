 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "IronEgg/AbilitySystem/SAbilitySystemComponent.h"
#include "IronEgg/AbilitySystem/SAttributeSet.h"
#include "SPlayerState.generated.h"

/**
 *
 */
UCLASS()
class IRONEGG_API ASPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ASPlayerState(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(blueprintcallable)
	USAbilitySystemComponent* GetSAbilitySystemComponent() const ;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	USAttributeSet * GetSAttributeSet () const;
	
protected:
	
private:

	TObjectPtr<USAbilitySystemComponent> AbilitySystemComponent;

	TObjectPtr<USAttributeSet> AttributeSet;
};
