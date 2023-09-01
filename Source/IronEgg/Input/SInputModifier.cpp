// Fill out your copyright notice in the Description page of Project Settings.


#include "SInputModifier.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SInputModifier)

DEFINE_LOG_CATEGORY_STATIC(LogSInputModifiers, Log, All);



FInputActionValue USSettingBasedScalar::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (ensureMsgf(CurrentValue.GetValueType() != EInputActionValueType::Boolean, TEXT("Setting Based Scalar modifier doesn't support boolean values.")))
	{




		FVector ScalarToUse = FVector(1.0, 1.0, 1.0);


		ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);
		ScalarToUse.Y = FMath::Clamp(ScalarToUse.Y, MinValueClamp.Y, MaxValueClamp.Y);
		ScalarToUse.Z = FMath::Clamp(ScalarToUse.Z, MinValueClamp.Z, MaxValueClamp.Z);

		return CurrentValue.Get<FVector>() * ScalarToUse;
	}

	return CurrentValue;

}
