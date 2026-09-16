// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeOptionCycle.h"
#include "CodeOptionRotator.h"
#include "CodeMainWidgetButton.h"

void UCodeOptionCycle::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	T_OptionName->SetText(OptionNameText);
	InitializeOptionCycle();
}

void UCodeOptionCycle::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (LeftButton)
		LeftButton->OnClicked().AddUObject(this, &UCodeOptionCycle::OnLeftButtonClicked);
	
	if (RightButton)
		RightButton->OnClicked().AddUObject(this, &UCodeOptionCycle::OnRightButtonClicked);
	
}

void UCodeOptionCycle::OnLeftButtonClicked()
{
	bIsMarkedAsCustom = false;
	OptionText->ShiftTextLeft();
	OnOptionChanged.Broadcast(OptionText->GetSelectedIndex());
}

void UCodeOptionCycle::OnRightButtonClicked()
{
	bIsMarkedAsCustom = false;
	OptionText->ShiftTextRight();
	OnOptionChanged.Broadcast(OptionText->GetSelectedIndex());
}

void UCodeOptionCycle::UpdateSelected(const int32 SelectedIndex)
{
	OptionText->SetSelectedItem(SelectedIndex);
	OptionText->SetActivePipIndex(SelectedIndex);
	bIsMarkedAsCustom = false;
}

void UCodeOptionCycle::UpdateTextToCustom()
{
	OptionText->MarkAsCustom();
	bIsMarkedAsCustom = true;
}

int32 UCodeOptionCycle::GetSelectedIndex() const
{
	if (bIsMarkedAsCustom)
	{
		return -1;
	}
	else
	{
		return OptionText->GetSelectedIndex();
	}
}

void UCodeOptionCycle::InitializeOptionCycle()
{
	OptionText->PopulateTextLabels(OptionArray);
	OptionText->SetSelectedItem(DefaultSelectedIndex);
	OptionText->SetupPips(OptionArray.Num());

	if (DefaultSelectedIndex >= 0)
	{
		OptionText->SetActivePipIndex(DefaultSelectedIndex);
	}
	else
	{
		OptionText->MarkAsCustom();
	}

	T_OptionName->SetText(OptionNameText);
}
