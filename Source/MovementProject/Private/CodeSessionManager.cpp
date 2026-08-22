#include "CodeSessionManager.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "CodeMainMenuPlayerController.h"
#include "CodePlayerSaveGame.h"

const FString UCodeSessionManager::SaveSlotName = TEXT("CodePlayerSave");

IOnlineSessionPtr UCodeSessionManager::GetSessionInterface() const
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		return Subsystem->GetSessionInterface();
	}
	return nullptr;
}

void UCodeSessionManager::CreateLobby(const FString LobbyName, const int32 MaxPlayers, const FName MapName)
{
	const IOnlineSessionPtr SessionInterface = GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateLobby: SessionInterface invalid"));
		return;
	}

	PendingMapName = MapName;
	PendingMaxPlayers = FMath::Max(1, MaxPlayers);

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = PendingMaxPlayers;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.Set(FName("LOBBY_NAME"), LobbyName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCodeSessionManager::HandleCreateSessionComplete);
	SessionInterface->CreateSession(0, FName("WerewolfLobby"), SessionSettings);
}

void UCodeSessionManager::HandleCreateSessionComplete(FName SessionName, const bool bWasSuccessful) const
{
	if (bWasSuccessful)
	{
		if (ACodeMainMenuPlayerController* PlayerController = Cast<ACodeMainMenuPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PlayerController->ShowLoadingScreen();
			
			const FString TravelOptions = FString::Printf(TEXT("listen?MaxPlayers=%d?Name=%s"), PendingMaxPlayers, *LocalPlayerName);
			UGameplayStatics::OpenLevel(this, PendingMapName, true, TravelOptions);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleCreateSessionComplete: Failed to create session"));
	}
	OnCreateSessionCompleteEvent.Broadcast();
}

void UCodeSessionManager::FindLobbies()
{
	if (bSearchInProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindLobbies: Search already in progress"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("FindLobbies: Searching"));

	const IOnlineSessionPtr SessionInterface = GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}

	bSearchInProgress = true;
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 20;

	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCodeSessionManager::HandleFindSessionsComplete);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UCodeSessionManager::HandleFindSessionsComplete(const bool bWasSuccessful)
{
	bSearchInProgress = false;
	
	FoundLobbyNames.Empty();

	if (bWasSuccessful && SessionSearch.IsValid())
	{
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FString LobbyName;
			if (Result.Session.SessionSettings.Get(FName("LOBBY_NAME"), LobbyName))
			{
				FoundLobbyNames.Add(LobbyName);
			}
			else
			{
				FoundLobbyNames.Add(TEXT("Unnamed Lobby"));
			}
		}
	}

	OnSessionSearchCompleteEvent.Broadcast();
}

void UCodeSessionManager::JoinLobby(const int32 SearchResultIndex)
{
	const IOnlineSessionPtr SessionInterface = GetSessionInterface();
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		return;
	}

	if (!SessionSearch->SearchResults.IsValidIndex(SearchResultIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinLobby: Invalid index %d"), SearchResultIndex);
		return;
	}

	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCodeSessionManager::HandleJoinSessionComplete);
	SessionInterface->JoinSession(0, FName("WerewolfLobby"), SessionSearch->SearchResults[SearchResultIndex]);
}

void UCodeSessionManager::SetLocalPlayerName(const FString& NewName)
{
	FString Trimmed = NewName.TrimStartAndEnd();
	
	if (Trimmed.IsEmpty())
	{
		Trimmed = GenerateDefaultName();
	}
	
	LocalPlayerName = SanitizeNameForURL(Trimmed);
	SavePlayerName();
}

FString UCodeSessionManager::GetLocalPlayerName()
{
	if (LocalPlayerName.IsEmpty())
	{
		LoadPlayerName();
	}
	return LocalPlayerName;
}

void UCodeSessionManager::HandleJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result) const
{
	bool bSuccess = false;

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		const IOnlineSessionPtr SessionInterface = GetSessionInterface();
		FString ConnectString;
		if (SessionInterface.IsValid() && SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
		{
			ConnectString += FString::Printf(TEXT("?Name=%s"), *LocalPlayerName);
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				if (ACodeMainMenuPlayerController* PlayerController = Cast<ACodeMainMenuPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->ShowLoadingScreen();
					PC->ClientTravel(ConnectString, TRAVEL_Absolute);
					bSuccess = true;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleJoinSessionComplete: Failed with result %d"), (int32)Result);
	}

	OnJoinSessionCompleteEvent.Broadcast(bSuccess);
}

void UCodeSessionManager::LoadPlayerName()
{
	if (const UCodePlayerSaveGame* LoadedGame = Cast<UCodePlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		LocalPlayerName = LoadedGame->PlayerName;
	}
	
	if (LocalPlayerName.IsEmpty())
	{
		LocalPlayerName = GenerateDefaultName();
	}
}

void UCodeSessionManager::SavePlayerName() const
{
	if (UCodePlayerSaveGame* SaveGameInstance = Cast<UCodePlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UCodePlayerSaveGame::StaticClass())))
	{
		SaveGameInstance->PlayerName = LocalPlayerName;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	}
}

FString UCodeSessionManager::GenerateDefaultName()
{
	return FString::Printf(TEXT("Player%d"), FMath::RandRange(1000, 9999));
}

FString UCodeSessionManager::SanitizeNameForURL(const FString& InName)
{
	FString Result = InName;
	Result = Result.Replace(TEXT("?"), TEXT("")).Replace(TEXT("="),TEXT("")).Replace(TEXT("&"), TEXT(""));
	return Result.Left(20);
}
