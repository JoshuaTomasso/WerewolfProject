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
	
	UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
	if (CodeGameInstance && CodeGameInstance->SessionManager)
	{
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
	RemoveFromParent();
}

void UCodePlayGame::CreateSessionButtonClicked()
{
	if (SessionNameTextBox)
	{
		
		FString LobbyName = SessionNameTextBox->GetText().ToString();
		if (LobbyName.IsEmpty())
		{
			LobbyName = TEXT("Werewolf Lobby");
		}
		
		UCodeGameInstance* CodeGameInstance = GetCodeGameInstance();
		if (CodeGameInstance && CodeGameInstance->SessionManager)
		{
			CodeGameInstance->SessionManager->CreateLobby(LobbyName, 8, MapToOpen);
		}
		
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

class UCodeGameInstance* UCodePlayGame::GetCodeGameInstance() const
{
	return Cast<UCodeGameInstance>(GetGameInstance());
}
