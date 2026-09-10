// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeOptionKey.h"

void UCodeOptionKey::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	T_OptionName->SetText(DefaultInputName);
	MainKey->UpdateKey(DefaultInputKey);
	
}

void UCodeOptionKey::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
	MainKey->OnClicked().AddUObject(this, &UCodeOptionKey::OnKeyButtonClicked);
}

FReply UCodeOptionKey::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsListeningToKeyEvent)
	{
		MainKey->UpdateKey(InKeyEvent.GetKey());
		bIsListeningToKeyEvent = false;
		DefaultInputKey = InKeyEvent.GetKey();
		OnKeyChangedDispatch.Broadcast(FName(DefaultInputName.ToString()), DefaultInputKey);
		
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCodeOptionKey::OnKeyButtonClicked()
{
	MainKey->T_KeyName->SetText(FText::FromString("Press Any Key"));
	bIsListeningToKeyEvent = true;
	
	SetKeyboardFocus();
}

void UCodeOptionKey::SetUnAssigned()
{
	MainKey->UpdateKey(EKeys::Invalid);
}
