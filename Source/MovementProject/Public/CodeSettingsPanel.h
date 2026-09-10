// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CodeSettingsPanel.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeSettingsPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* AudioSettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* VideoSettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ControlsSettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DisplaySettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void AudioButtonClicked();
	
	UFUNCTION()
	void VideoButtonClicked();
	
	UFUNCTION()
	void DisplayButtonClicked();
	
	UFUNCTION()
	void ControlsButtonClicked();
	
	UFUNCTION()
	void BackButtonClicked();
	
};
