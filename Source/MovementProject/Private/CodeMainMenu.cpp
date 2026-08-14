// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenu.h"
#include "CodePlayGame.h"
#include "CodeMainMenuPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UCodeMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (PlayGameButton)
		PlayGameButton->OnPressed.AddDynamic(this, &UCodeMainMenu::PlayGameButtonClicked);
	
	if (SettingButton)
		SettingButton->OnPressed.AddDynamic(this, &UCodeMainMenu::SettingsButtonClicked);
	
	if (QuitButton)
		QuitButton->OnPressed.AddDynamic(this, &UCodeMainMenu::QuitButtonClicked);
	
}

void UCodeMainMenu::PlayGameButtonClicked()
{
	if (PlayGameButton)
	{
		PlayGameWidget = CreateWidget<UCodePlayGame>(GetWorld(), CodePlayGameClass);
		if (PlayGameWidget)
		{
			PlayGameWidget->OnPlayGameClosed.AddDynamic(this, &UCodeMainMenu::ShowMainMenuButtons);
			PlayGameWidget->AddToViewport();
			MainMenuHolder->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}

void UCodeMainMenu::SettingsButtonClicked()
{
	// Settings Widget
}

void UCodeMainMenu::QuitButtonClicked()
{
	if (ACodeMainMenuPlayerController* PlayerController = Cast<ACodeMainMenuPlayerController>(GetOwningPlayer()))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

void UCodeMainMenu::ShowMainMenuButtons()
{
	MainMenuHolder->SetVisibility(ESlateVisibility::Visible);
}
