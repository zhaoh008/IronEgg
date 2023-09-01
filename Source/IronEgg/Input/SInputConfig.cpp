// Fill out your copyright notice in the Description page of Project Settings.


#include "SInputConfig.h"
#include "GameplayTagContainer.h"
#include "../SLogChannels.h"

USInputConfig::USInputConfig(const FObjectInitializer& ObjectInitializer)
{

}

const UInputAction* USInputConfig::GetInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound /*= true*/) const
{
	for (const FSInputAction& InputAction : InputActions)
	{
		if (InputAction.InputTag.operator==(InputTag))
		{
			return InputAction.InputAction.Get();
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogEgg, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}


