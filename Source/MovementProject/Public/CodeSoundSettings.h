// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "CodeSoundWidget.h"
#include "CodeOptionCycle.h"
#include "CodeSoundSettings.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeSoundSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SoundSettingScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	class UDataTable* SoundConfigDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	TSubclassOf<UCodeSoundWidget> SoundWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	UCodeSoundWidget* SoundWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeOptionCycle> OptionCycleWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* OptionCycleWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	TMap<FName, UCodeSoundWidget*> VolumeSliders;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	FText AudioDeviceOptionLabel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	TArray<FAudioOutputDeviceInfo> AudioDevices;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void ResetButtonClicked();
	
	UFUNCTION()
	void ApplyButtonClicked();
	
	UFUNCTION()
	void BackButtonClicked();
	
	UFUNCTION()
	void SetupSoundSettings();
	
	UFUNCTION()
	void SetupAudioDeviceOption(const TArray<FAudioOutputDeviceInfo>& AvailableDevices);
	
	UFUNCTION()
	void VolumeSliderValueChanged(float Value);
	
	UFUNCTION()
	void ChangeAudioDevice(int32 SelectedIndex);
	
	UFUNCTION()
	void OnAudioDeviceSwapped(const FSwapAudioOutputResult& SwapResult);

	UFUNCTION()
	void ApplySoundSettings();
	
	UFUNCTION()
	void ResetVolumes();
};
