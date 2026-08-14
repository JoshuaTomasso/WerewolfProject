// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CodeLobbyEntry.generated.h"

class UCodeSessionManager;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeLobbyEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LobbyNameText;
	
	UFUNCTION(BlueprintCallable)
	void SetupEntry(const FString& LobbyName, int32 InSearchIndex, UCodeSessionManager* InSessionManager);
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	UCodeSessionManager* SessionManager;
	
	int32 SearchIndex;
	
	UFUNCTION()
	void OnJoinButtonPressed();
	
};
