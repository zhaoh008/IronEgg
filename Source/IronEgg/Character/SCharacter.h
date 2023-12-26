// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "InputMappingContext.h"
#include "Components/WidgetComponent.h"
#include "SCharacter.generated.h"

class USAbilitySystemComponent;
class UCameraComponent;
class USpringArmComponent;
class USInputConfig;
UCLASS()
class IRONEGG_API ASCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:

	ASCharacter();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		USInputConfig* SInputConfig;

	UPROPERTY(EditDefaultsOnly,Category="Health")
	TSubclassOf< class UUserWidget> HealthWidget;

	UPROPERTY(EditDefaultsOnly,Category="Health")
	UWidgetComponent * WidgetComponent;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

	void Input_Jump(const FInputActionValue& InputActionValue);

public:

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void OnRep_PlayerState() override;
	
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable,Category="Ability")
	USAbilitySystemComponent * GetSAbilitySystemComponent() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStateClient_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossesseServer_Implementation();
protected:

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

   virtual void	InitializeFloatingStatusBar();
	
	TWeakObjectPtr<class USAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class USAttributeSet> AttributeSetBase;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class USGameplayAbility>> CharacterAbilities;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
	
};
