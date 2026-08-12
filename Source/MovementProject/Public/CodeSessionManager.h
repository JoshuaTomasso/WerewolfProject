#pragma once
#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CodeSessionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreateComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbySearchComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyJoinComplete, bool, bSuccess);

UCLASS(Blueprintable)
class MOVEMENTPROJECT_API UCodeSessionManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateLobby(FString LobbyName, int32 MaxPlayers, FName MapName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindLobbies();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void JoinLobby(int32 SearchResultIndex);

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbyCreateComplete OnCreateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbySearchComplete OnSessionSearchCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnLobbyJoinComplete OnJoinSessionCompleteEvent;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	TArray<FString> FoundLobbyNames;

private:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName PendingMapName;

	void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void HandleFindSessionsComplete(bool bWasSuccessful);
	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;

	IOnlineSessionPtr GetSessionInterface() const;
};