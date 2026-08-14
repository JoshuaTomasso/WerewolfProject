// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodeNightResult.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeNightResult : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NightResultText;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateNightResult(const FText& ResultText) const;

	UFUNCTION()
	void ShowNightResult();

	UFUNCTION()
	void HideNightResult() const;

private:

	FTimerHandle InitializationTimerHandle;
	FTimerHandle NightResultTimerHandle;

	int32 InitializationRetryCount;

	UFUNCTION()
	void TryInitializePlayerState();
};
