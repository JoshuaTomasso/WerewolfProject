// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSettingsPanel.h"
#include "CodeMainMenu.h"


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
}

void UCodeSettingsPanel::VideoButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::GraphicsSettings);
	}
}

void UCodeSettingsPanel::DisplayButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::DisplaySettings);
	}
}

void UCodeSettingsPanel::ControlsButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::ControlSettings);
	}
}

void UCodeSettingsPanel::BackButtonClicked()
{
	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::MainButtons);
	}
}
