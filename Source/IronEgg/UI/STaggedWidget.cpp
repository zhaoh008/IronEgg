// Fill out your copyright notice in the Description page of Project Settings.


#include "STaggedWidget.h"

USTaggedWidget::USTaggedWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void USTaggedWidget::SetVisibility(ESlateVisibility InVisibility)
{
	
	#if WITH_EDITORONLY_DATA
		if (IsDesignTime())
		{
			Super::SetVisibility(InVisibility);
			return;
		}
	#endif

		// Remember what the caller requested; even if we're currently being
		// suppressed by a tag we should respect this call when we're done
		bWantsToBeVisible = ConvertSerializedVisibilityToRuntime(InVisibility).IsVisible();
		if (bWantsToBeVisible)
		{
			ShownVisibility = InVisibility;
		}
		else
		{
			HiddenVisibility = InVisibility;
		}

		const bool bHasHiddenTags = false;//@TODO: Foo->HasAnyTags(HiddenByTags);

		// Actually apply the visibility
		const ESlateVisibility DesiredVisibility = (bWantsToBeVisible && !bHasHiddenTags) ? ShownVisibility : HiddenVisibility;
		if (GetVisibility() != DesiredVisibility)
		{
			Super::SetVisibility(DesiredVisibility);
		}
}

void USTaggedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!IsDesignTime())
	{
		SetVisibility(GetVisibility());		
	}
}

void USTaggedWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USTaggedWidget::OnWatchedTagsChanged()
{
	const bool bHasHiddenTags = false;//@TODO: Foo->HasAnyTags(HiddenByTags);

	// Actually apply the visibility
	const ESlateVisibility DesiredVisibility = (bWantsToBeVisible && !bHasHiddenTags) ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		Super::SetVisibility(DesiredVisibility);
	}
}


