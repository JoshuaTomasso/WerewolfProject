// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeQuestionScreen.h"

void UCodeQuestionScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	MainQuestion->SetText(MainQuestionText);
	ButtonOne->MB_ChangeableText = ButtonOneText;
	ButtonTwo->MB_ChangeableText = ButtonTwoText;
	
}

void UCodeQuestionScreen::SetQuestionText(const FText& Text)
{
	UE_LOG(LogTemp, Warning, TEXT("SetQuestionText called with: %s"), *Text.ToString());
       
	MainQuestion->SetText(Text);
}

void UCodeQuestionScreen::SetButtonCount(const bool bNeedsTwoButtons)
{
	if (!bNeedsTwoButtons)
	{
		ButtonTwo->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCodeQuestionScreen::SetButtonOneText(const FText& Text)
{
	ButtonOne->SetButtonText(Text);
}

void UCodeQuestionScreen::SetButtonTwoText(const FText& Text)
{
	ButtonTwo->SetButtonText(Text);
}
