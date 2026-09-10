// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSoundSettings.h"
#include "CodeMainMenu.h"
#include "SSoundConfigs.h"
#include "VectorUtil.h"
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
}

void UCodeSoundSettings::SetupSoundSettings()
{
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UHiddenFangUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UHiddenFangUserSettingsSubsystem>())
		{
			TMap<FName, float> TempVolumeData = SettingsSubsystem->LoadSettings()->AudioSettings.VolumeData;
	
			for (const FName RowName : SoundConfigDataTable->GetRowNames())
			{
				if (const FSSoundConfigs* SettingsRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(RowName, TEXT("SetupSoundSettings")))
				{
					SoundWidget = CreateWidget<UCodeSoundWidget>(GetOwningPlayer(), SoundWidgetClass);
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
					
					UGameplayStatics::PushSoundMixModifier(this, SettingsRow->SoundMix);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Row Found For SettingsRow"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No SubSystem Found"));
		}
	}
}

void UCodeSoundSettings::SetupAudioDeviceOption(const TArray<FAudioOutputDeviceInfo>& AvailableDevices)
{
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
	OptionCycleWidget = CreateWidget<UCodeOptionCycle>(GetOwningPlayer(), OptionCycleWidgetClass);
	OptionCycleWidget->OptionArray = AudioDeviceList;
	OptionCycleWidget->DefaultSelectedIndex = ActiveDeviceIndex;
	OptionCycleWidget->OptionNameText = AudioDeviceOptionLabel;
	OptionCycleWidget->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
	SoundSettingScrollBox->AddChild(OptionCycleWidget);
	
	OptionCycleWidget->OnOptionChanged.AddDynamic(this, &UCodeSoundSettings::ChangeAudioDevice);
}

void UCodeSoundSettings::VolumeSliderValueChanged(float Value)
{
	for (const TPair<FName, UCodeSoundWidget*> VolumePair : VolumeSliders)
	{
		if (const FSSoundConfigs* VolumeRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(VolumePair.Key, TEXT("VolumeSliderValueChanged")))
		{
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
		TempVolumeData.Add(VolumePair.Key, VolumePair.Value->GetVolumeValue());
	}
	
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UHiddenFangUserSettingsSubsystem* SettingsSubsystem = GameInstance->GetSubsystem<UHiddenFangUserSettingsSubsystem>())
		{
			FSAudioSettings AudioSettings;
			AudioSettings.VolumeData = TempVolumeData;
			
			SettingsSubsystem->SaveAudioSettings(AudioSettings);
		}
	}
}

void UCodeSoundSettings::ResetVolumes()
{
	for (const FName RowName : SoundConfigDataTable->GetRowNames())
	{
		if (const FSSoundConfigs* SettingsRow = SoundConfigDataTable->FindRow<FSSoundConfigs>(RowName, TEXT("SetupSoundSettings")))
		{
			if (UCodeSoundWidget* Temp = *VolumeSliders.Find(RowName))
			{
				Temp->UpdateSlider(SettingsRow->DefaultVolume);
			}
		}
	}
}
