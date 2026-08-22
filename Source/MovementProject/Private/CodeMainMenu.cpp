// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenu.h"


void UCodeMainMenu::ShowPanel(ECodeMainMenuPanelOrder PanelOrder)
{
	if (MainMenuWidgetSwitcher)
	{
		MainMenuWidgetSwitcher->SetActiveWidgetIndex(static_cast<int32>(PanelOrder));
	}
}

void UCodeMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ShowPanel(ECodeMainMenuPanelOrder::MainButtons);
}


