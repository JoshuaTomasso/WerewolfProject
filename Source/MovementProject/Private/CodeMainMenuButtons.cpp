// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenuButtons.h"

#include "CodeMainMenu.h"
#include "CodePlayGame.h"
#include "CodeMainMenuPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UCodeMainMenuButtons::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (PlayGameButton)
		PlayGameButton->OnPressed.AddDynamic(this, &UCodeMainMenuButtons::PlayGameButtonClicked);
	
	if (SettingButton)
		SettingButton->OnPressed.AddDynamic(this, &UCodeMainMenuButtons::SettingsButtonClicked);
	
	if (QuitButton)
		QuitButton->OnPressed.AddDynamic(this, &UCodeMainMenuButtons::QuitButtonClicked);
	
}

void UCodeMainMenuButtons::PlayGameButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::PlayGame);
	}
}

void UCodeMainMenuButtons::SettingsButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SettingsPanel);
	}
}

void UCodeMainMenuButtons::QuitButtonClicked()
{
	if (ACodeMainMenuPlayerController* PlayerController = Cast<ACodeMainMenuPlayerController>(GetOwningPlayer()))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
