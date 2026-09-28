// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeCommonButtonWithText.h"

void UCodeCommonButtonWithText::SetButtonText(const FText& NewText)
{
	DefaultText = NewText;
	if (T_TextBlock)
	{
		T_TextBlock->SetText(NewText);
	}
}

void UCodeCommonButtonWithText::NativeConstruct()
{
	Super::NativeConstruct();
	
	T_TextBlock->SetText(DefaultText);
	
}
