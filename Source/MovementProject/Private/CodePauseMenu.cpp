// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePauseMenu.h"

void UCodePauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowPanel(ECodePauseMenuPanelOrder::PauseMenu);
	
	if (PauseMenuWidgetSwitcher)
	{
		if (UWidget* ActiveWidget = PauseMenuWidgetSwitcher->GetActiveWidget())
		{
			SetDesiredFocusWidget(ActiveWidget);
		}
	}
}

void UCodePauseMenu::ShowPanel(ECodePauseMenuPanelOrder PanelOrder)
{
	if (PauseMenuWidgetSwitcher)
	{
		PauseMenuWidgetSwitcher->SetActiveWidgetIndex(static_cast<int32>(PanelOrder));
		if (UWidget* NewActive = PauseMenuWidgetSwitcher->GetActiveWidget())
		{
			SetDesiredFocusWidget(NewActive);
			NewActive->SetFocus();
		}
	}
}

void UCodePauseMenu::ClosePauseMenu()
{
	RemoveFromParent();
}
