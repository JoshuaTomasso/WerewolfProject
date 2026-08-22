// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "CodeNightActionTargeting.generated.h"

class ACodePlayerState;
class UCodeButtonAndText;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeNightActionTargeting : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerChoicesScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PartnerChoiceText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChosenPlayerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* VoteNotificationText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	ACodePlayerState* PartnersTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	ACodePlayerState* CastedPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UCodeButtonAndText> ButtonAndTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	float EntrySpacing = 10.0f;

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void PopulateTargetList();

	UFUNCTION()
	void UpdatePartnerVoteDisplay();

	UFUNCTION()
	void UpdatePartnerDisplay();

	UFUNCTION()
	void OnRoleAssignedHandler();

	UFUNCTION()
	void ShowVoteNotification();

	UFUNCTION()
	void HideVoteNotification();
	
	UFUNCTION()
	void RefreshPlayerNameText();


private:
	FTimerHandle InitializationTimerHandle;

	int32 InitializationRetryCount;

	UFUNCTION()
	void TryInitializePlayerState();

	UFUNCTION()
	void InitializePlayerState();
};
