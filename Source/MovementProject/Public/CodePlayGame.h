// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SpinBox.h"
#include "CodePlayGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayGameClosed);

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodePlayGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* SessionNameTextBox;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* PlayerCountSpinBox;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CloseWidgetButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateSessionButton;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SessionsScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PlayerNameTextBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	FName MapToOpen = FName("CodeTestMap");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	int32 MinPlayerCount = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	int32 MaxPlayerCount = 16;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	int32 DefaultPlayerCount = 8;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UCodeLobbyEntry> LobbyEntryWidgetClass;
	
	UPROPERTY(BlueprintAssignable, Category = "Widget")
	FOnPlayGameClosed OnPlayGameClosed;
	
protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:
	
	UFUNCTION()
	void CloseButtonClicked();
	
	UFUNCTION()
	void CreateSessionButtonClicked();
	
	UFUNCTION()
	void RefreshLobbyList();
	
	UFUNCTION()
	void OnRefreshButtonPressed();
	
	UFUNCTION()
	void OnPlayerNameCommitted(const FText& NewText, ETextCommit::Type CommitMethod);
	
private:
	
	class UCodeGameInstance* GetCodeGameInstance() const;
	
	
};
