// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSettingsPanel.h"
#include "CodeMainMenu.h"
#include "CodePauseMenu.h"


void UCodeSettingsPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (AudioSettingsButton)
		AudioSettingsButton->OnPressed.AddDynamic(this, &UCodeSettingsPanel::AudioButtonClicked);
	
	if (VideoSettingsButton)
		VideoSettingsButton->OnPressed.AddDynamic(this, &UCodeSettingsPanel::VideoButtonClicked);
	
	if (DisplaySettingsButton)
		DisplaySettingsButton->OnPressed.AddDynamic(this, &UCodeSettingsPanel::DisplayButtonClicked);
	
	if (ControlsSettingsButton)
		ControlsSettingsButton->OnPressed.AddDynamic(this, &UCodeSettingsPanel::ControlsButtonClicked);
	
	if (BackButton)
		BackButton->OnPressed.AddDynamic(this, &UCodeSettingsPanel::BackButtonClicked);
}

void UCodeSettingsPanel::AudioButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::SoundSettings);
	}
	else if (UCodePauseMenu* PauseParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		PauseParentMenu->ShowPanel(ECodePauseMenuPanelOrder::SoundPanel);
	}
}

void UCodeSettingsPanel::VideoButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::GraphicsSettings);
	}
	else if (UCodePauseMenu* PauseParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		PauseParentMenu->ShowPanel(ECodePauseMenuPanelOrder::GraphicsPanel);
	}
}

void UCodeSettingsPanel::DisplayButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::DisplaySettings);
	}
	else if (UCodePauseMenu* PauseParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		PauseParentMenu->ShowPanel(ECodePauseMenuPanelOrder::DisplayPanel);
	}
	
}

void UCodeSettingsPanel::ControlsButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::ControlSettings);
	}
	else if (UCodePauseMenu* PauseParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		PauseParentMenu->ShowPanel(ECodePauseMenuPanelOrder::ControlPanel);
	}
}

void UCodeSettingsPanel::BackButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::MainButtons);
	}
	else if (UCodePauseMenu* PauseParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		PauseParentMenu->ShowPanel(ECodePauseMenuPanelOrder::PauseMenu);
	}
}
