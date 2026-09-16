// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSoundSettings.h"
#include "CodeMainMenu.h"
#include "SSoundConfigs.h"
#include "Kismet/GameplayStatics.h"
#include "HiddenFangUserSettingsSubsystem.h"
#include "SAudioSettings.h"

void UCodeSoundSettings::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ResetButton)
		ResetButton->OnPressed.AddDynamic(this, &UCodeSoundSettings::ResetButtonClicked);

	if (ApplyButton)
		ApplyButton->OnPressed.AddDynamic(this, &UCodeSoundSettings::ApplyButtonClicked);

	if (BackButton)
		BackButton->OnPressed.AddDynamic(this, &UCodeSoundSettings::BackButtonClicked);

	if (!SoundSettingScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeConstruct: SoundSettingScrollBox is null!"));
		return;
	}

	SoundSettingScrollBox->ClearChildren();
	
	SetupSoundSettings();

	FOnAudioOutputDevicesObtained DevicesObtainedCallback;
	DevicesObtainedCallback.BindDynamic(this, &UCodeSoundSettings::SetupAudioDeviceOption);
	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
	
}

void UCodeSoundSettings::ResetButtonClicked()
{
	ResetVolumes();
}

void UCodeSoundSettings::ApplyButtonClicked()
{
	ApplySoundSettings();
}

void UCodeSoundSettings::BackButtonClicked()
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

void UCodeSoundSettings::SetupSoundSettings()
{
	if (!SoundConfigDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: SoundConfigDataTable is null!"));
		return;
	}

	if (!SoundSettingScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: SoundSettingScrollBox is null!"));
		return;
	}

	if (!SoundWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: SoundWidgetClass is null!"));
		return;
	}

	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: GameInstance is null!"));
		return;
	}

	UHiddenFangUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UHiddenFangUserSettingsSubsystem>();
	if (!SettingsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("No SubSystem Found"));
		return;
	}

	UCodeUserSettings* UserSettings = SettingsSubsystem->LoadSettings();
	if (!UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: LoadSettings() returned null!"));
		return;
	}

	TMap<FName, float> TempVolumeData;

	if (&UserSettings->AudioSettings != nullptr)
	{
		TempVolumeData = UserSettings->AudioSettings.VolumeData;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupSoundSettings: AudioSettings structure is uninitialized. Using empty fallback."));
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: GetOwningPlayer() returned null!"));
		return;
	}

	for (const FName RowName : SoundConfigDataTable->GetRowNames())
	{
		if (const FSSoundConfigs* SettingsRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(RowName, TEXT("SetupSoundSettings")))
		{
			SoundWidget = CreateWidget<UCodeSoundWidget>(OwningPlayer, SoundWidgetClass);
			if (!SoundWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("SetupSoundSettings: Failed to create SoundWidget for row %s"), *RowName.ToString());
				continue;
			}

			SoundWidget->SoundName = SettingsRow->SoundClassName;
			SoundWidget->DefaultSliderValue = SettingsRow->DefaultVolume;
			VolumeSliders.Add(RowName, SoundWidget);
			SoundWidget->OnVolumeSliderValueChanged.AddDynamic(this, &UCodeSoundSettings::VolumeSliderValueChanged);
			SoundWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
			SoundSettingScrollBox->AddChild(SoundWidget);
			
			if (const float* FoundValue = TempVolumeData.Find(RowName))
			{
				SoundWidget->UpdateSlider(*FoundValue);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Volume Data Found"));
			}

			if (SettingsRow->SoundMix)
			{
				UGameplayStatics::PushSoundMixModifier(this, SettingsRow->SoundMix);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("SetupSoundSettings: SoundMix is null for row %s"), *RowName.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Row Found For SettingsRow"));
		}
	}
}

void UCodeSoundSettings::SetupAudioDeviceOption(const TArray<FAudioOutputDeviceInfo>& AvailableDevices)
{
	if (!OptionCycleWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAudioDeviceOption: OptionCycleWidgetClass is null!"));
		return;
	}

	if (!SoundSettingScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAudioDeviceOption: SoundSettingScrollBox is null!"));
		return;
	}

	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAudioDeviceOption: GetOwningPlayer() returned null!"));
		return;
	}

	TArray<FText> AudioDeviceList;
	int32 ActiveDeviceIndex = 0;
	AudioDevices = AvailableDevices;
	OptionCycleWidget = nullptr;
	
	for (int32 i = 0; i < AvailableDevices.Num(); ++i)
	{
		const FAudioOutputDeviceInfo& AvailableDevice = AvailableDevices[i];

		AudioDeviceList.Add(FText::FromString(AvailableDevice.Name));

		if (AvailableDevice.bIsCurrentDevice)
		{
			ActiveDeviceIndex = i;
		}
	}

	OptionCycleWidget = CreateWidget<UCodeOptionCycle>(OwningPlayer, OptionCycleWidgetClass);
	if (!OptionCycleWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SetupAudioDeviceOption: Failed to create OptionCycleWidget!"));
		return;
	}

	OptionCycleWidget->OptionArray = AudioDeviceList;
	OptionCycleWidget->DefaultSelectedIndex = ActiveDeviceIndex;
	OptionCycleWidget->OptionNameText = AudioDeviceOptionLabel;
	OptionCycleWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
	SoundSettingScrollBox->AddChild(OptionCycleWidget);
	
	OptionCycleWidget->OnOptionChanged.AddDynamic(this, &UCodeSoundSettings::ChangeAudioDevice);
}

void UCodeSoundSettings::VolumeSliderValueChanged(float Value)
{
	if (!SoundConfigDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("VolumeSliderValueChanged: SoundConfigDataTable is null!"));
		return;
	}

	for (const TPair<FName, UCodeSoundWidget*> VolumePair : VolumeSliders)
	{
		if (!VolumePair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("VolumeSliderValueChanged: Null widget for row %s"), *VolumePair.Key.ToString());
			continue;
		}

		if (const FSSoundConfigs* VolumeRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(VolumePair.Key, TEXT("VolumeSliderValueChanged")))
		{
			if (!VolumeRow->SoundMix || !VolumeRow->SoundClass)
			{
				UE_LOG(LogTemp, Warning, TEXT("VolumeSliderValueChanged: SoundMix or SoundClass is null for row %s"), *VolumePair.Key.ToString());
				continue;
			}

			UGameplayStatics::SetSoundMixClassOverride(
			   this,
			   VolumeRow->SoundMix,       
			   VolumeRow->SoundClass,     
			   FMath::Clamp(VolumePair.Value->GetVolumeValue(), 0.01f, 1.0f),
			   1.0f,
			   1.0f,
			   true 
			);
			
		}
	}
}

void UCodeSoundSettings::ChangeAudioDevice(int32 SelectedIndex)
{
	if (!AudioDevices.IsValidIndex(SelectedIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeAudioDevice: invalid index %d"), SelectedIndex);
		return;
	}

	const FString NewDeviceId = AudioDevices[SelectedIndex].DeviceId;

	FOnCompletedDeviceSwap CompletedCallback;
	CompletedCallback.BindDynamic(this, &UCodeSoundSettings::OnAudioDeviceSwapped);

	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(this, NewDeviceId, CompletedCallback);
}

void UCodeSoundSettings::OnAudioDeviceSwapped(const FSwapAudioOutputResult& SwapResult)
{
	UE_LOG(LogTemp, Log, TEXT("Audio device swap completed. Success: %s"),
	  SwapResult.Result == ESwapAudioOutputDeviceResultState::Success ? TEXT("true") : TEXT("false"));
}

void UCodeSoundSettings::ApplySoundSettings()
{
	TMap<FName, float> TempVolumeData;
	
	for (TPair<FName, UCodeSoundWidget*> VolumePair : VolumeSliders)
	{
		if (!VolumePair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplySoundSettings: Null widget for row %s"), *VolumePair.Key.ToString());
			continue;
		}

		TempVolumeData.Add(VolumePair.Key, VolumePair.Value->GetVolumeValue());
	}
	
	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplySoundSettings: GameInstance is null!"));
		return;
	}

	UHiddenFangUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UHiddenFangUserSettingsSubsystem>();
	if (!SettingsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplySoundSettings: No SubSystem Found"));
		return;
	}

	FSAudioSettings AudioSettings;
	AudioSettings.VolumeData = TempVolumeData;
	
	SettingsSubsystem->SaveAudioSettings(AudioSettings);
}

void UCodeSoundSettings::ResetVolumes()
{
	if (!SoundConfigDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ResetVolumes: SoundConfigDataTable is null!"));
		return;
	}

	for (const FName RowName : SoundConfigDataTable->GetRowNames())
	{
		if (const FSSoundConfigs* SettingsRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(RowName, TEXT("SetupSoundSettings")))
		{
			if (UCodeSoundWidget** FoundWidget = VolumeSliders.Find(RowName))
			{
				if (UCodeSoundWidget* Temp = *FoundWidget)
				{
					Temp->UpdateSlider(SettingsRow->DefaultVolume);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ResetVolumes: Null widget for row %s"), *RowName.ToString());
				}
			}
		}
	}
}