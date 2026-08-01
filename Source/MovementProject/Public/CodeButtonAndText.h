// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CodeButtonAndText.generated.h"

class ACodePlayerState;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeButtonAndText : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UButton* PlayerNameButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	ACodePlayerState* targetPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor ButtonClickedColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor BaseButtonColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float ButtonResetDelay;

	FTimerHandle ButtonResetTimerHandle;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SetupEntry(ACodePlayerState* PlayerReference);

	UFUNCTION()
	void OnButtonPressed();

	UFUNCTION()
	void UpdateVoteCountDisplay();

	UFUNCTION()
	void ResetButtonColor();
};
