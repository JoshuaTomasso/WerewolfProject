// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeLobbyEntry.h"
#include "CodeSessionManager.h"

void UCodeLobbyEntry::SetupEntry(const FString& LobbyName, int32 InSearchIndex, UCodeSessionManager* InSessionManager)
{
	SearchIndex = InSearchIndex;
	SessionManager = InSessionManager;
	
	if (LobbyNameText)
		LobbyNameText->SetText(FText::FromString(LobbyName));
}

void UCodeLobbyEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (JoinButton)
		JoinButton->OnPressed.AddDynamic(this, &UCodeLobbyEntry::OnJoinButtonPressed);
}

void UCodeLobbyEntry::OnJoinButtonPressed()
{
	if (SessionManager)
		SessionManager->JoinLobby(SearchIndex);
}
