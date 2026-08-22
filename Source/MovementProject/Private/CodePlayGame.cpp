// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePlayGame.h"
#include "CodeGameInstance.h"
#include "CodeSessionManager.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CodeLobbyEntry.h"
#include "CodeMainMenu.h"

void UCodePlayGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseWidgetButton)
		CloseWidgetButton->OnPressed.AddDynamic(this, &UCodePlayGame::CloseButtonClicked);
	
	if (CreateSessionButton)
		CreateSessionButton->OnPressed.AddDynamic(this, &UCodePlayGame::CreateSessionButtonClicked);
	
	if (RefreshButton)
		RefreshButton->OnPressed.AddDynamic(this, &UCodePlayGame::OnRefreshButtonPressed);
	
	if (PlayerCountSpinBox)
	{
		PlayerCountSpinBox->SetMinValue(MinPlayerCount);
	 	PlayerCountSpinBox->SetMaxValue(MaxPlayerCount);
		PlayerCountSpinBox->SetMinSliderValue(MinPlayerCount);
		PlayerCountSpinBox->SetMaxSliderValue(MaxPlayerCount);
		PlayerCountSpinBox->SetValue(DefaultPlayerCount);
	}
	
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
		if (PlayerNameTextBox)
		{
			PlayerNameTextBox->SetText(FText::FromString(CodeGameInstance->SessionManager->GetLocalPlayerName()));
			PlayerNameTextBox->OnTextCommitted.AddDynamic(this, &UCodePlayGame::OnPlayerNameCommitted);
		}
		
		CodeGameInstance->SessionManager->OnSessionSearchCompleteEvent.RemoveDynamic(this, &UCodePlayGame::RefreshLobbyList);
		CodeGameInstance->SessionManager->OnSessionSearchCompleteEvent.AddDynamic(this, &UCodePlayGame::RefreshLobbyList);
		
		CodeGameInstance->SessionManager->FindLobbies();
	}
}

void UCodePlayGame::NativeDestruct()
{
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
		CodeGameInstance->SessionManager->OnSessionSearchCompleteEvent.RemoveDynamic(this, &UCodePlayGame::RefreshLobbyList);	
	}
	
	Super::NativeDestruct();
}

void UCodePlayGame::CloseButtonClicked()
{
	OnPlayGameClosed.Broadcast();

	if (UCodeMainMenu* ParentMenu = GetTypedOuter<UCodeMainMenu>())
	{
		ParentMenu->ShowPanel(ECodeMainMenuPanelOrder::MainButtons);
	}
}

void UCodePlayGame::CreateSessionButtonClicked()
{
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
		// Catch the case where the player typed a name but never lost focus / committed
		if (PlayerNameTextBox)
		{
			CodeGameInstance->SessionManager->SetLocalPlayerName(PlayerNameTextBox->GetText().ToString());
		}

		FString LobbyName = SessionNameTextBox ? SessionNameTextBox->GetText().ToString() : FString();
		if (LobbyName.IsEmpty() || LobbyName.Equals(TEXT("SESSION NAME"), ESearchCase::IgnoreCase))
		{
			LobbyName = TEXT("Werewolf Lobby");
		}

		int32 MaxPlayers = DefaultPlayerCount;
		if (PlayerCountSpinBox)
		{
			MaxPlayers = FMath::Clamp(FMath::RoundToInt(PlayerCountSpinBox->GetValue()), MinPlayerCount, MaxPlayerCount);
		}

		CodeGameInstance->SessionManager->CreateLobby(LobbyName, MaxPlayers, MapToOpen);
	}
}

void UCodePlayGame::RefreshLobbyList()
{
	if (SessionsScrollBox)
	{
		SessionsScrollBox->ClearChildren();
		
		UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
		if (CodeGameInstance && CodeGameInstance->SessionManager && LobbyEntryWidgetClass)
		{
			for (int32 i = 0; i < CodeGameInstance->SessionManager->FoundLobbyNames.Num(); i++)
			{
				if (UCodeLobbyEntry* Entry = CreateWidget<UCodeLobbyEntry>(this, LobbyEntryWidgetClass))
				{
					Entry->SetupEntry(CodeGameInstance->SessionManager->FoundLobbyNames[i], i, CodeGameInstance->SessionManager);
					SessionsScrollBox->AddChild(Entry);
					UE_LOG(LogTemp, Warning, TEXT("Refresh Lobby List: List Refreshed"));
				}
			}
		}
		
	}
}

void UCodePlayGame::OnRefreshButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh Lobby List : Button pressed"));
	
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
		CodeGameInstance->SessionManager->FindLobbies();
		RefreshLobbyList();
	}
}

void UCodePlayGame::OnPlayerNameCommitted(const FText& NewText, ETextCommit::Type CommitMethod)
{
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
		CodeGameInstance->SessionManager->SetLocalPlayerName(NewText.ToString());
		
		if (PlayerNameTextBox)
		{
			PlayerNameTextBox->SetText(FText::FromString(CodeGameInstance->SessionManager->GetLocalPlayerName()));
		}
	}
}

class UCodeGameInstance* UCodePlayGame::GetCodeGameInstance() const
{
	return Cast<UCodeGameInstance>(GetGameInstance());
}
