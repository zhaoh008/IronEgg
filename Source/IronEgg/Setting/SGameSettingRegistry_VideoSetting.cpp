#include "GameSettingAction.h"
#include "GameSettingCollection.h"
#include "GameSettingValueDiscreteDynamic.h"
#include "GameSettingValueScalarDynamic.h"
#include "SGameSettingRegistry.h"
#include "SSettingsLocal.h"
#include "CustomSettings/LyraSettingValueDiscrete_Resolution.h"
#include "EditCondition/WhenCondition.h"
#include "EditCondition/WhenPlatformHasTrait.h"
#include "NativeGameplayTags.h"
#include "CustomSettings/LyraSettingValueDiscrete_OverallQuality.h"
#include "IronEgg/Performance/LyraPerformanceSettings.h"


#define  LOCTEXT_NAMESPACE "IronEgg"
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_SupportsWindowedMode, "Platform.Trait.SupportsWindowedMode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_NeedsBrightnessAdjustment, "Platform.Trait.NeedsBrightnessAdjustment");

UE_DEFINE_GAMEPLAY_TAG_STATIC(GameSettings_Action_EditSafeZone, "GameSettings.Action.EditSafeZone");
UE_DEFINE_GAMEPLAY_TAG_STATIC(GameSettings_Action_EditBrightness, "GameSettings.Action.EditBrightness");


//////////////////////////////////////////////////////////////////////

enum class EFramePacingEditCondition
{
	EnableIf,
	DisableIf
};

// Checks the platform-specific value for FramePacingMode
class FGameSettingEditCondition_FramePacingMode : public FGameSettingEditCondition
{
public:
	FGameSettingEditCondition_FramePacingMode(ELyraFramePacingMode InDesiredMode, EFramePacingEditCondition InMatchMode = EFramePacingEditCondition::EnableIf)
		: DesiredMode(InDesiredMode)
		, MatchMode(InMatchMode)
	{
	}

	virtual void GatherEditState(const ULocalPlayer * InLocalPlayer, FGameSettingEditableState & InOutEditState) const override
	{
		const ELyraFramePacingMode ActualMode = ULyraPlatformSpecificRenderingSettings::Get()->FramePacingMode;
		
		const bool bMatches = (ActualMode == DesiredMode);
		const bool bMatchesAreBad = (MatchMode == EFramePacingEditCondition::DisableIf);

		if (bMatches == bMatchesAreBad)
		{
			InOutEditState.Kill(FString::Printf(TEXT("Frame pacing mode %d didn't match requirement %d"), (int32)ActualMode, (int32)DesiredMode));
		}
	}
private:
	ELyraFramePacingMode DesiredMode;
	EFramePacingEditCondition MatchMode;
};

//////////////////////////////////////////////////////////////////////

// Checks the platform-specific value for bSupportsGranularVideoQualitySettings
class FGameSettingEditCondition_VideoQuality : public FGameSettingEditCondition
{
public:
	FGameSettingEditCondition_VideoQuality(const FString& InDisableString)
		: DisableString(InDisableString)
	{
	}

	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGameSettingEditableState& InOutEditState) const override
	{
		if (!ULyraPlatformSpecificRenderingSettings::Get()->bSupportsGranularVideoQualitySettings)
		{
			InOutEditState.Kill(DisableString);
		}
	}

	virtual void SettingChanged(const ULocalPlayer* LocalPlayer, UGameSetting* Setting, EGameSettingChangeReason Reason) const override
	{
		// TODO for now this applies the setting immediately
		const USLocalPlayer* LyraLocalPlayer = CastChecked<USLocalPlayer>(LocalPlayer);
		LyraLocalPlayer->GetLocalSettings()->ApplyScalabilitySettings();
	}

private:
	FString DisableString;
};

UGameSettingCollection* USGameSettingRegistry::InitializeVideoSettings(USLocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("VideoCollection"));
	Screen->SetDisplayName(LOCTEXT("VideoCollection_Name", "Video"));
	Screen->Initialize(InLocalPlayer);

	UGameSettingValueDiscreteDynamic_Enum* WindowModeSetting = nullptr;
	UGameSetting* MobileFPSType = nullptr;

	// Display
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingCollection* Display = NewObject<UGameSettingCollection>();
		Display->SetDevName(TEXT("DisplayCollection"));
		Display->SetDisplayName(LOCTEXT("DisplayCollection_Name", "Display"));
		Screen->AddSetting(Display);

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Enum* Setting = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			Setting->SetDevName(TEXT("WindowMode"));
			Setting->SetDisplayName(LOCTEXT("WindowMode_Name", "Window Mode"));
			Setting->SetDescriptionRichText(LOCTEXT("WindowMode_Description", "In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetFullscreenMode));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetFullscreenMode));
			Setting->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("WindowModeFullscreen", "Fullscreen"));
			Setting->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("WindowModeWindowedFullscreen", "Windowed Fullscreen"));
			Setting->AddEnumOption(EWindowMode::Windowed, LOCTEXT("WindowModeWindowed", "Windowed"));

			Setting->AddEditCondition(FWhenPlatformHasTrait::KillIfMissing(TAG_Platform_Trait_SupportsWindowedMode, TEXT("Platform does not support window mode")));

			WindowModeSetting = Setting;

			Display->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			ULyraSettingValueDiscrete_Resolution* Setting = NewObject<ULyraSettingValueDiscrete_Resolution>();
			Setting->SetDevName(TEXT("Resolution"));
			Setting->SetDisplayName(LOCTEXT("Resolution_Name", "Resolution"));
			Setting->SetDescriptionRichText(LOCTEXT("Resolution_Description", "Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			Setting->AddEditDependency(WindowModeSetting);
			Setting->AddEditCondition(FWhenPlatformHasTrait::KillIfMissing(TAG_Platform_Trait_SupportsWindowedMode, TEXT("Platform does not support window mode")));
			Setting->AddEditCondition(MakeShared<FWhenCondition>([WindowModeSetting](const ULocalPlayer*, FGameSettingEditableState& InOutEditState)
			{
				if (WindowModeSetting->GetValue<EWindowMode::Type>() == EWindowMode::WindowedFullscreen)
				{
					InOutEditState.Disable(LOCTEXT("ResolutionWindowedFullscreen_Disabled", "When the Window Mode is set to <strong>Windowed Fullscreen</>, the resolution must match the native desktop resolution."));
				}
			}));

			Display->AddSetting(Setting);
		}
	}

	// Graphics Quality
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingCollection* GraphicsQuality = NewObject<UGameSettingCollection>();
		GraphicsQuality->SetDevName(TEXT("GraphicsQuality"));
		GraphicsQuality->SetDisplayName(LOCTEXT("GraphicsQuality_Name", "Graphics Quality"));
		Screen->AddSetting(GraphicsQuality);

		UGameSetting* AutoSetQuality = nullptr;
		UGameSetting* GraphicsQualityPresets = nullptr;

			//----------------------------------------------------------------------------------
		{
			UGameSettingAction* Setting = NewObject<UGameSettingAction>();
			Setting->SetDevName(TEXT("AutoSetQuality"));
			Setting->SetDisplayName(LOCTEXT("AutoSetQuality_Name", "Auto-Set Quality"));
			Setting->SetDescriptionRichText(LOCTEXT("AutoSetQuality_Description", "Automatically configure the graphics quality options based on a benchmark of the hardware."));

			Setting->SetDoesActionDirtySettings(true);
			Setting->SetActionText(LOCTEXT("AutoSetQuality_Action", "Auto-Set"));
			Setting->SetCustomAction([](ULocalPlayer* LocalPlayer)
			{
				const ULyraPlatformSpecificRenderingSettings* PlatformSettings = ULyraPlatformSpecificRenderingSettings::Get();
				if (PlatformSettings->FramePacingMode == ELyraFramePacingMode::MobileStyle)
				{
					//USSettingsLocal::Get()->ResetToMobileDeviceDefaults();
				}
				else
				{
					const USLocalPlayer* UsLocalPlayer = CastChecked<USLocalPlayer>(LocalPlayer);
					// We don't save state until users apply the settings.
					constexpr bool bImmediatelySaveState = false;
					UsLocalPlayer->GetLocalSettings()->RunAutoBenchmark(bImmediatelySaveState);
				}
			});

			Setting->AddEditCondition(MakeShared<FWhenCondition>([](const ULocalPlayer* LocalPlayer, FGameSettingEditableState& InOutEditState)
			{
				const ULyraPlatformSpecificRenderingSettings* PlatformSettings = ULyraPlatformSpecificRenderingSettings::Get();
				const bool bCanUseDueToMobile = (PlatformSettings->FramePacingMode == ELyraFramePacingMode::MobileStyle);

				const USLocalPlayer* LyraLocalPlayer = CastChecked<USLocalPlayer>(LocalPlayer);
				const bool bCanBenchmark = LyraLocalPlayer->GetLocalSettings()->CanRunAutoBenchmark();

				if (!bCanUseDueToMobile && !bCanBenchmark)
				{
					InOutEditState.Kill(TEXT("Auto quality not supported"));
				}
			}));

			if (MobileFPSType != nullptr)
			{
				MobileFPSType->AddEditDependency(Setting);
			}

			GraphicsQuality->AddSetting(Setting);

			AutoSetQuality = Setting;
		}
		//----------------------------------------------------------------------------------
		{
			ULyraSettingValueDiscrete_OverallQuality* Setting = NewObject<ULyraSettingValueDiscrete_OverallQuality>();
			Setting->SetDevName(TEXT("GraphicsQualityPresets"));
			Setting->SetDisplayName(LOCTEXT("GraphicsQualityPresets_Name", "Quality Presets"));
			Setting->SetDescriptionRichText(LOCTEXT("GraphicsQualityPresets_Description", "Quality Preset allows you to adjust multiple video options at once. Try a few options to see what fits your preference and device's performance."));

			Setting->AddEditDependency(AutoSetQuality);

			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_FramePacingMode>(ELyraFramePacingMode::ConsoleStyle, EFramePacingEditCondition::DisableIf));

			if (MobileFPSType != nullptr)
			{
				Setting->AddEditDependency(MobileFPSType);
				MobileFPSType->AddEditDependency(Setting);
			}

			GraphicsQuality->AddSetting(Setting);

			GraphicsQualityPresets = Setting;
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
			Setting->SetDevName(TEXT("ResolutionScale"));
			Setting->SetDisplayName(LOCTEXT("ResolutionScale_Name", "3D Resolution"));
			Setting->SetDescriptionRichText(LOCTEXT("ResolutionScale_Description", "3D resolution determines the resolution that objects are rendered in game, but does not affect the main menu.  Lower resolutions can significantly increase frame rate."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetResolutionScaleNormalized));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetResolutionScaleNormalized));
			Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support 3D Resolution")));
			//@TODO: Add support for 3d res on mobile

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);
			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("GlobalIlluminationQuality"));
			Setting->SetDisplayName(LOCTEXT("GlobalIlluminationQuality_Name", "Global Illumination"));
			Setting->SetDescriptionRichText(LOCTEXT("GlobalIlluminationQuality_Description", "Global Illumination controls the quality of dynamically calculated indirect lighting bounces, sky shadowing and Ambient Occlusion. Settings of 'High' and above use more accurate ray tracing methods to solve lighting, but can reduce performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetGlobalIlluminationQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetGlobalIlluminationQuality));
			Setting->AddOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
			Setting->AddOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
			Setting->AddOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support GlobalIlluminationQuality")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("Shadows"));
			Setting->SetDisplayName(LOCTEXT("Shadows_Name", "Shadows"));
			Setting->SetDescriptionRichText(LOCTEXT("Shadows_Description", "Shadow quality determines the resolution and view distance of dynamic shadows. Shadows improve visual quality and give better depth perception, but can reduce performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetShadowQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetShadowQuality));
			Setting->AddOption(0, LOCTEXT("ShadowLow", "Off"));
			Setting->AddOption(1, LOCTEXT("ShadowMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("ShadowHigh", "High"));
			Setting->AddOption(3, LOCTEXT("ShadowEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support Shadows")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("AntiAliasing"));
			Setting->SetDisplayName(LOCTEXT("AntiAliasing_Name", "Anti-Aliasing"));
			Setting->SetDescriptionRichText(LOCTEXT("AntiAliasing_Description", "Anti-Aliasing reduces jaggy artifacts along geometry edges. Increasing this setting will make edges look smoother, but can reduce performance. Higher settings mean more anti-aliasing."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetAntiAliasingQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetAntiAliasingQuality));
			Setting->AddOption(0, LOCTEXT("AntiAliasingLow", "Off"));
			Setting->AddOption(1, LOCTEXT("AntiAliasingMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("AntiAliasingHigh", "High"));
			Setting->AddOption(3, LOCTEXT("AntiAliasingEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support Anti-Aliasing")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("ViewDistance"));
			Setting->SetDisplayName(LOCTEXT("ViewDistance_Name", "View Distance"));
			Setting->SetDescriptionRichText(LOCTEXT("ViewDistance_Description", "View distance determines how far away objects are culled for performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetViewDistanceQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetViewDistanceQuality));
			Setting->AddOption(0, LOCTEXT("ViewDistanceNear", "Near"));
			Setting->AddOption(1, LOCTEXT("ViewDistanceMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("ViewDistanceFar", "Far"));
			Setting->AddOption(3, LOCTEXT("ViewDistanceEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support View Distance")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("TextureQuality"));
			Setting->SetDisplayName(LOCTEXT("TextureQuality_Name", "Textures"));

			Setting->SetDescriptionRichText(LOCTEXT("TextureQuality_Description", "Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetTextureQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetTextureQuality));
			Setting->AddOption(0, LOCTEXT("TextureQualityLow", "Low"));
			Setting->AddOption(1, LOCTEXT("TextureQualityMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("TextureQualityHigh", "High"));
			Setting->AddOption(3, LOCTEXT("TextureQualityEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support Texture quality")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("VisualEffectQuality"));
			Setting->SetDisplayName(LOCTEXT("VisualEffectQuality_Name", "Effects"));
			Setting->SetDescriptionRichText(LOCTEXT("VisualEffectQuality_Description", "Effects determines the quality of visual effects and lighting in game. Increasing this setting will increase the quality of visual effects, but can reduce performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetVisualEffectQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetVisualEffectQuality));
			Setting->AddOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
			Setting->AddOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
			Setting->AddOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support VisualEffectQuality")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("ReflectionQuality"));
			Setting->SetDisplayName(LOCTEXT("ReflectionQuality_Name", "Reflections"));
			Setting->SetDescriptionRichText(LOCTEXT("ReflectionQuality_Description", "Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetReflectionQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetReflectionQuality));
			Setting->AddOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
			Setting->AddOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
			Setting->AddOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support ReflectionQuality")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* Setting = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			Setting->SetDevName(TEXT("PostProcessingQuality"));
			Setting->SetDisplayName(LOCTEXT("PostProcessingQuality_Name", "Post Processing"));
			Setting->SetDescriptionRichText(LOCTEXT("PostProcessingQuality_Description", "Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));  

			Setting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetPostProcessingQuality));
			Setting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetPostProcessingQuality));
			Setting->AddOption(0, LOCTEXT("PostProcessingQualityLow", "Low"));
			Setting->AddOption(1, LOCTEXT("PostProcessingQualityMedium", "Medium"));
			Setting->AddOption(2, LOCTEXT("PostProcessingQualityHigh", "High"));
			Setting->AddOption(3, LOCTEXT("PostProcessingQualityEpic", "Epic"));

			Setting->AddEditDependency(AutoSetQuality);
			Setting->AddEditDependency(GraphicsQualityPresets);
			Setting->AddEditCondition(MakeShared<FGameSettingEditCondition_VideoQuality>(TEXT("Platform does not support PostProcessingQuality")));

			// When this setting changes, it can GraphicsQualityPresets to be set to custom, or a particular preset.
			GraphicsQualityPresets->AddEditDependency(Setting);

			GraphicsQuality->AddSetting(Setting);
		}

	}
	return Screen;
}

#undef	 LOCTEXT_NAMESPACE