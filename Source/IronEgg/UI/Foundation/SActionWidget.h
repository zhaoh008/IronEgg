// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "UObject/Object.h"
#include "SActionWidget.generated.h"
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class IRONEGG_API USActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()
public:
	
	//~ Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~ End of UCommonActionWidget interface

	/** The Enhanced Input Action that is associated with this Common Input action. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	const TObjectPtr<UInputAction> AssociatedInputAction;

private:

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
	
};
