// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePauseMenuButtons.h"
#include "CodePauseMenu.h"
#include "CodeSessionManager.h"
#include "Kismet/GameplayStatics.h"


void UCodePauseMenuButtons::NativeConstruct()
{
	Super::NativeConstruct();
	
	
	if (ContinueGameButton)
		ContinueGameButton->SetButtonText(ContinueDefaultText);

	if (MainMenuButton)
		MainMenuButton->SetButtonText(MainMenuDefaultText);

	if (SettingsButton)
		SettingsButton->SetButtonText(SettingsDefaultText);

	if (ContinueGameButton)
		ContinueGameButton->OnClicked().AddUObject(this, &UCodePauseMenuButtons::ContinueGameButtonClicked);
	if (MainMenuButton)
		MainMenuButton->OnClicked().AddUObject(this, &UCodePauseMenuButtons::MainMenuButtonClicked);
	if (SettingsButton)
		SettingsButton->OnClicked().AddUObject(this, &UCodePauseMenuButtons::SettingsButtonClicked);
}

void UCodePauseMenuButtons::ContinueGameButtonClicked()
{
	if (UCodePauseMenu* ParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		ParentMenu->ClosePauseMenu();
	}
}

void UCodePauseMenuButtons::MainMenuButtonClicked()
{
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCodeSessionManager* SessionManager = GameInstance->GetSubsystem<UCodeSessionManager>())
		{
			SessionManager->LeaveLobby();
			return;
		}
	}

	const FString TravelOptions = FString::Printf(TEXT("Moving To Main Menu"));
	UGameplayStatics::OpenLevel(this, PendingMapName, true, TravelOptions);
}

void UCodePauseMenuButtons::SettingsButtonClicked()
{
	if (UCodePauseMenu* ParentMenu = GetTypedOuter<UCodePauseMenu>())
	{
		ParentMenu->ShowPanel(ECodePauseMenuPanelOrder::SettingsPanel);
	}
}
