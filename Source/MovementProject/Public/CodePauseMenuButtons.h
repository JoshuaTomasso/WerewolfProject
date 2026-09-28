// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeCommonButtonWithText.h"
#include "CodePauseMenuButtons.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodePauseMenuButtons : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UCodeCommonButtonWithText* ContinueGameButton;
	
	UPROPERTY(meta = (BindWidget))
	UCodeCommonButtonWithText* MainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UCodeCommonButtonWithText* SettingsButton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FText ContinueDefaultText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FText MainMenuDefaultText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FText SettingsDefaultText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FName PendingMapName;
	
protected:
	
	virtual void NativeConstruct() override;

public:
	
	UFUNCTION()
	void ContinueGameButtonClicked();
	
	UFUNCTION()
	void MainMenuButtonClicked();
	
	UFUNCTION()
	void SettingsButtonClicked();
	
};
