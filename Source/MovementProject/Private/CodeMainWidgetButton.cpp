// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainWidgetButton.h"

void UCodeMainWidgetButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	MB_Text->SetText(MB_ChangeableText);
	
}

void UCodeMainWidgetButton::SetButtonText(const FText& Text)
{
	MB_Text->SetText(Text);
}
