// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CodeMainMenuButtons.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeMainMenuButtons : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UButton* PlayGameButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
protected:
	
	virtual void NativeConstruct();
	
public:
	
	UFUNCTION()
	void PlayGameButtonClicked();
	
	UFUNCTION()
	void SettingsButtonClicked();
	
	UFUNCTION()
	void QuitButtonClicked();

};
