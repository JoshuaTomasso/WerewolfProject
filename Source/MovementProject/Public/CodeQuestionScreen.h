// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodeMainWidgetButton.h"
#include "CodeQuestionScreen.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeQuestionScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainQuestion;
	
	UPROPERTY(meta = (BindWidget))
	UCodeMainWidgetButton* ButtonOne;
	
	UPROPERTY(meta = (BindWidget))
	UCodeMainWidgetButton* ButtonTwo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText MainQuestionText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ButtonOneText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ButtonTwoText;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	
	UFUNCTION()
	void SetQuestionText(const FText& Text);
	
	UFUNCTION()
	void SetButtonCount(bool bNeedsTwoButtons);
	
	UFUNCTION()
	void SetButtonOneText(const FText& Text);
	
	UFUNCTION()
	void SetButtonTwoText(const FText& Text);
};
