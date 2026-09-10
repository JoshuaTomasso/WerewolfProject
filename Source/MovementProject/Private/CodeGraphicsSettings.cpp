// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGraphicsSettings.h"
#include "CodeMainMenu.h"
#include "CodeOptionCycle.h"
#include "GameFramework/GameUserSettings.h"

void UCodeGraphicsSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ResetButton)
		ResetButton->OnPressed.AddDynamic(this, &UCodeGraphicsSettings::ResetButtonClicked);

	if (ApplyButton)
		ApplyButton->OnPressed.AddDynamic(this, &UCodeGraphicsSettings::ApplyButtonClicked);

	if (BackButton)
		BackButton->OnPressed.AddDynamic(this, &UCodeGraphicsSettings::BackButtonClicked);
	
	SetupGraphicsSettings();
}



void UCodeGraphicsSettings::ResetButtonClicked()
{
	ResetOptions();
}

void UCodeGraphicsSettings::ApplyButtonClicked()
{
	ApplyOptions();
}

void UCodeGraphicsSettings::BackButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
}

void UCodeGraphicsSettings::SetupGraphicsSettings()
{
	AllOptions.Empty();
	OverallOption = nullptr;
	SettingsScrollBox->ClearChildren();
	for (const FName RowName : GraphicsSettingsDataTable->GetRowNames())
	{
		if (const FCodeProjectGraphicsConfig* SettingsRow = GraphicsSettingsDataTable->FindRow<FCodeProjectGraphicsConfig>(RowName, TEXT("SetupGraphicsSettings")))
		{
			OptionCycleWidget = CreateWidget<UCodeOptionCycle>(GetOwningPlayer(), OptionCycleWidgetClass);
			SettingsRow->Options.GenerateValueArray(OptionCycleWidget->OptionArray );
			TArray<int32> OptionIndexes;
			SettingsRow->Options.GenerateKeyArray(OptionIndexes);
			
			const int32 QualityValue = GetOptionValue(SettingsRow->GraphicsOptionType);
			OptionCycleWidget->DefaultSelectedIndex = OptionIndexes.IsValidIndex(QualityValue) ? OptionIndexes[QualityValue] : -1;
			
			OptionCycleWidget->OptionNameText = StaticEnum<ECodeProjectGraphicOptions>()->GetDisplayNameTextByValue(static_cast<int64>(SettingsRow->GraphicsOptionType));
			OptionCycleWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
			SettingsScrollBox->AddChild(OptionCycleWidget);
			
			AllOptions.Add(SettingsRow->GraphicsOptionType, OptionCycleWidget);
			
			if (SettingsRow->GraphicsOptionType == ECodeProjectGraphicOptions::Overall)
			{
				OverallOption = OptionCycleWidget;
			}
			else
			{
				OptionCycleWidget->OnOptionChanged.AddDynamic(this, &UCodeGraphicsSettings::OnNonOverallOptionChanged);
			}
		}
	}
	
	OverallOption->OnOptionChanged.AddDynamic(this, &UCodeGraphicsSettings::ChangeOverallOptions);
}

void UCodeGraphicsSettings::ChangeOverallOptions(const int32 SelectedIndex)
{
	for (const TPair<ECodeProjectGraphicOptions, UCodeOptionCycle*>& OptionPair : AllOptions)
	{
		if (OptionPair.Key != ECodeProjectGraphicOptions::Overall)
		{
			OptionPair.Value->UpdateSelected(SelectedIndex);
		}
	}
}

void UCodeGraphicsSettings::OnNonOverallOptionChanged(int32 SelectedIndex)
{
	OverallOption->UpdateTextToCustom();
}

void UCodeGraphicsSettings::ApplyOptions()
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("No User Settings"));
		return;
	}
	
	for (const TPair<ECodeProjectGraphicOptions, UCodeOptionCycle*>& OptionPair : AllOptions)
	{
		if (OptionPair.Value->GetSelectedIndex() >= 0)
		{
			switch (OptionPair.Key)
			{
			case ECodeProjectGraphicOptions::Overall:
				GameUserSettings->SetOverallScalabilityLevel(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::ViewDistance:
				GameUserSettings->SetViewDistanceQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::AntiAliasingQuality:
				GameUserSettings->SetAntiAliasingQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::ShadowQuality:
				GameUserSettings->SetShadowQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::GlobalIlluminationQuality:
				GameUserSettings->SetGlobalIlluminationQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::ReflectionQuality:
				GameUserSettings->SetReflectionQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::PostProcessQuality:
				GameUserSettings->SetPostProcessingQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::TextureQuality:
				GameUserSettings->SetTextureQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::EffectsQuality:
				GameUserSettings->SetVisualEffectQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::FoliageQuality:
				GameUserSettings->SetFoliageQuality(OptionPair.Value->GetSelectedIndex());
				break;
			case ECodeProjectGraphicOptions::ShadingQuality:
				GameUserSettings->SetShadingQuality(OptionPair.Value->GetSelectedIndex());
				break;
			}
		}
	}
}

void UCodeGraphicsSettings::ResetOptions()
{
	OverallOption->UpdateSelected(DefaultOverallIndex);
	ChangeOverallOptions(DefaultOverallIndex);
}

int32 UCodeGraphicsSettings::GetOptionValue(ECodeProjectGraphicOptions GraphicOptionType) const
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("No User Settings"));
		return 0;
	}
	
	switch (GraphicOptionType)
	{
	case ECodeProjectGraphicOptions::Overall:
		return GameUserSettings->GetOverallScalabilityLevel();
	case ECodeProjectGraphicOptions::ViewDistance:
		return GameUserSettings->GetViewDistanceQuality();
	case ECodeProjectGraphicOptions::AntiAliasingQuality:
		return GameUserSettings->GetAntiAliasingQuality();
	case ECodeProjectGraphicOptions::ShadowQuality:
		return GameUserSettings->GetShadowQuality();
	case ECodeProjectGraphicOptions::GlobalIlluminationQuality:
		return GameUserSettings->GetGlobalIlluminationQuality();
	case ECodeProjectGraphicOptions::ReflectionQuality:
		return GameUserSettings->GetReflectionQuality();
	case ECodeProjectGraphicOptions::PostProcessQuality:
		return GameUserSettings->GetPostProcessingQuality();
	case ECodeProjectGraphicOptions::TextureQuality:
		return GameUserSettings->GetTextureQuality();
	case ECodeProjectGraphicOptions::EffectsQuality:
		return GameUserSettings->GetVisualEffectQuality();
	case ECodeProjectGraphicOptions::FoliageQuality:
		return GameUserSettings->GetFoliageQuality();
	case ECodeProjectGraphicOptions::ShadingQuality:
		return GameUserSettings->GetShadingQuality();
	}
	return 0;
}
