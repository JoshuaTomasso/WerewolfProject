// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenFangUserSettingsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "CodeUserSettings.h"

void UHiddenFangUserSettingsSubsystem::SaveAudioSettings(const FSAudioSettings& AudioSettings)
{
	LoadSettings()->AudioSettings = AudioSettings;
	SaveSettings();
}

UCodeUserSettings* UHiddenFangUserSettingsSubsystem::LoadSettings()
{
	if (IsValid(SGUserSettings))
	{
		return SGUserSettings;
	}

	if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlot, 0))
	{
		SGUserSettings = Cast<UCodeUserSettings>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlot, 0));
	}
	else
	{
		SGUserSettings = Cast<UCodeUserSettings>(UGameplayStatics::CreateSaveGameObject(SGUserSettingsClass));
	}
	
	return SGUserSettings;
}

void UHiddenFangUserSettingsSubsystem::SaveSettings()
{
	UGameplayStatics::SaveGameToSlot(SGUserSettings, SettingsSaveSlot, 0);
}

