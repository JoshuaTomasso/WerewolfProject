// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SAudioSettings.h"
#include "CodeUserSettings.h"
#include "HiddenFangUserSettingsSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MOVEMENTPROJECT_API UHiddenFangUserSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UHiddenFangUserSettingsSubsystem();
	
	UPROPERTY()
	UCodeUserSettings* SGUserSettings;
	
	UPROPERTY()
	FString SettingsSaveSlot = "UserSettings";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	TSubclassOf<USaveGame> SGUserSettingsClass;
	
	UFUNCTION()
	void SaveAudioSettings(const FSAudioSettings& AudioSettings);
	
	UFUNCTION()
	UCodeUserSettings* LoadSettings();
	
	UFUNCTION()
	void SaveSettings();
	
};
