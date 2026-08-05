// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CodeGamePhaseTimer.generated.h"

/**
 *
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeGamePhaseTimer : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* GamePhaseTimerBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GamePhaseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor lobbyColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor nightColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor dayColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor votingColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor roleRevealColor;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};