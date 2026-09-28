#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CodeSessionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreateComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbySearchComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyLeaveComplete, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyJoinComplete, bool, bSuccess);

UCLASS()
class MOVEMENTPROJECT_API UCodeSessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateLobby(FString LobbyName, int32 MaxPlayers, FName MapName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindLobbies();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void JoinLobby(int32 SearchResultIndex);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void SetLocalPlayerName(const FString& NewName);
	
	UFUNCTION(BlueprintCallable, Category = "Session")
	void LeaveLobby();
	
	UFUNCTION(BlueprintCallable, Category = "Session")
	FString GetLocalPlayerName();
	
	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbyCreateComplete OnCreateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbySearchComplete OnSessionSearchCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbyLeaveComplete OnLeaveSessionCompleteEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbyJoinComplete OnJoinSessionCompleteEvent;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	TArray<FString> FoundLobbyNames;
	
private:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString PendingLobbyName;
	FName PendingMapName;
	int32 PendingMaxPlayers = 8;
	bool bSearchInProgress = false;
	FString LocalPlayerName;
	static const FString SaveSlotName;
	
	void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void HandleFindSessionsComplete(bool bWasSuccessful);
	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;
	void HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void HandleDestroySessionThenCreate(FName SessionName, bool bWasSuccessful);
	
	void CreateLobbyInternal();
	void LoadPlayerName();
	void SavePlayerName() const;
	static FString GenerateDefaultName();
	static FString SanitizeNameForURL(const FString& InName);
	
	
	IOnlineSessionPtr GetSessionInterface() const;
};