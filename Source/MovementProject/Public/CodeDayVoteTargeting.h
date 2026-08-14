// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "CodeDayVoteTargeting.generated.h"

class UCodeButtonAndText;
class ACodePlayerState;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeDayVoteTargeting : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerChoicesScrollBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UCodeButtonAndText* SkipVoteButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UCodeButtonAndText> ButtonAndTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	float EntrySpacing = 10.0f;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void PopulateTargetList();

private:

	FTimerHandle InitializationTimerHandle;

	int32 InitializationRetryCount;

	UFUNCTION()
	void TryInitializePlayerState();
};
