// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "CodeMainMenu.generated.h"

class UCodePlayGame;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainMenuHolder;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PlayGameButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UCodePlayGame> CodePlayGameClass;
	
	UPROPERTY()
	UCodePlayGame* PlayGameWidget;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	
	UFUNCTION()
	void PlayGameButtonClicked();
	
	UFUNCTION()
	void SettingsButtonClicked();
	
	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void ShowMainMenuButtons();
	
};
