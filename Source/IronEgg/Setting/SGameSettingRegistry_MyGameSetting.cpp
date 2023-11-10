#include "SGameSettingRegistry.h"
#include "GameSetting.h"
#include "GameSettingValueScalar.h"
#include "GameSettingCollection.h"
#include "CustomSettings/SSettingValueDiscrete_MyGameSetting.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "IronEgg/Setting/CustomSettings/SSettingValueDiscrete_Language.h"
#include "IronEgg/Player/SLocalPlayer.h"

#define LOCTEXT_NAMESPACE "IronEgg"

UGameSettingCollection* USGameSettingRegistry::InitializeMyGameSettingCollection(USLocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("MyGameSettingCollection"));
	Screen->SetDisplayName(LOCTEXT("MyGameSettingCollection_Name", "MyGameSetting"));
	Screen->Initialize(InLocalPlayer);

	{
		UGameSettingCollection* Items = NewObject<UGameSettingCollection>();
		Items->SetDevName(TEXT("MyGameSettingItems"));
		Items->SetDisplayName(LOCTEXT("MyGameSettingItem_Name", "Items"));
		Screen->AddSetting(Items);
		{
			USSettingValueDiscrete_MyGameSetting* Setting=NewObject<USSettingValueDiscrete_MyGameSetting>();
			Setting->SetDevName(TEXT("MyGameSettingItem1"));
			Setting->SetDisplayName(LOCTEXT("MyGameSettingItem1_Name", "Item"));
			Setting->SetDescriptionRichText(LOCTEXT("MyGameSettingItem_Description", "1这是一个描述懂么,一个设置项的描述"));
			Setting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());
			Items->AddSetting(Setting);
		}
	}
	
	return  Screen;
}
#undef LOCTEXT_NAMESPACE
