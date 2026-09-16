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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BackButtonClicked: No UCodeMainMenu outer found!"));
	}
}

void UCodeGraphicsSettings::SetupGraphicsSettings()
{
	if (!GraphicsSettingsDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: GraphicsSettingsDataTable is null!"));
		return;
	}

	if (!SettingsScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: SettingsScrollBox is null!"));
		return;
	}

	if (!OptionCycleWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: OptionCycleWidgetClass is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: GetOwningPlayer() returned null!"));
		return;
	}

	const UEnum* GraphicOptionsEnum = StaticEnum<ECodeProjectGraphicOptions>();
	if (!GraphicOptionsEnum)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: ECodeProjectGraphicOptions enum is null!"));
		return;
	}

	AllOptions.Empty();
	OverallOption = nullptr;
	SettingsScrollBox->ClearChildren();

	for (const FName RowName : GraphicsSettingsDataTable->GetRowNames())
	{
		if (const FCodeProjectGraphicsConfig* SettingsRow = GraphicsSettingsDataTable->FindRow<FCodeProjectGraphicsConfig>(RowName, TEXT("SetupGraphicsSettings")))
		{
			OptionCycleWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
			if (!OptionCycleWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: Failed to create OptionCycleWidget for row %s"), *RowName.ToString());
				continue;
			}

			SettingsRow->Options.GenerateValueArray(OptionCycleWidget->OptionArray);
			TArray<int32> OptionIndexes;
			SettingsRow->Options.GenerateKeyArray(OptionIndexes);
			
			const int32 QualityValue = GetOptionValue(SettingsRow->GraphicsOptionType);
			OptionCycleWidget->DefaultSelectedIndex = OptionIndexes.IsValidIndex(QualityValue) ? OptionIndexes[QualityValue] : -1;
			
			OptionCycleWidget->OptionNameText = GraphicOptionsEnum->GetDisplayNameTextByValue(static_cast<int64>(SettingsRow->GraphicsOptionType));
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SetupGraphicsSettings: No row found for %s"), *RowName.ToString());
		}
	}
	
	if (OverallOption)
	{
		OverallOption->OnOptionChanged.AddDynamic(this, &UCodeGraphicsSettings::ChangeOverallOptions);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetupGraphicsSettings: No Overall option row found — OverallOption is null!"));
	}
}

void UCodeGraphicsSettings::ChangeOverallOptions(const int32 SelectedIndex)
{
	for (const TPair<ECodeProjectGraphicOptions, UCodeOptionCycle*>& OptionPair : AllOptions)
	{
		if (!OptionPair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("ChangeOverallOptions: Null widget in AllOptions!"));
			continue;
		}

		if (OptionPair.Key != ECodeProjectGraphicOptions::Overall)
		{
			OptionPair.Value->UpdateSelected(SelectedIndex);
		}
	}
}

void UCodeGraphicsSettings::OnNonOverallOptionChanged(int32 SelectedIndex)
{
	if (!OverallOption)
	{
		UE_LOG(LogTemp, Error, TEXT("OnNonOverallOptionChanged: OverallOption is null!"));
		return;
	}

	OverallOption->UpdateTextToCustom();
}

void UCodeGraphicsSettings::ApplyOptions()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyOptions: GEngine is null!"));
		return;
	}

	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogTemp, Warning, TEXT("No User Settings"));
		return;
	}
	
	for (const TPair<ECodeProjectGraphicOptions, UCodeOptionCycle*>& OptionPair : AllOptions)
	{
		if (!OptionPair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyOptions: Null widget in AllOptions for option key!"));
			continue;
		}

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
	if (!OverallOption)
	{
		UE_LOG(LogTemp, Error, TEXT("ResetOptions: OverallOption is null!"));
		return;
	}

	OverallOption->UpdateSelected(DefaultOverallIndex);
	ChangeOverallOptions(DefaultOverallIndex);
}

int32 UCodeGraphicsSettings::GetOptionValue(ECodeProjectGraphicOptions GraphicOptionType) const
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GetOptionValue: GEngine is null!"));
		return 0;
	}

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