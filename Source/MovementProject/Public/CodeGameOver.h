// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodeGameOver.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeGameOver : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameOverText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor werewolfWinnerColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor villagerWinnerColor;
	
};
