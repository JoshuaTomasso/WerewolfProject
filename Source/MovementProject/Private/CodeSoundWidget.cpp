// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSoundWidget.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCodeSoundWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetMainSliderImage();
	T_OptionName->SetText(SoundName);
	ValueSlider->SetValue(DefaultSliderValue);
	ValueText->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(DefaultSliderValue * 100))));
	OnSliderChangedValue(DefaultSliderValue);
	ValueSlider->OnValueChanged.AddDynamic(this, &UCodeSoundWidget::OnSliderChangedValue);
}

void UCodeSoundWidget::SetMainSliderImage()
{
	if (!ValueSlider || !SliderFillParentMaterial)
		return;
	
	DM_SliderFill = UMaterialInstanceDynamic::Create(SliderFillParentMaterial, this);
	
	FSliderStyle SliderStyle = ValueSlider->GetWidgetStyle();
	
	SliderStyle.NormalBarImage.SetResourceObject(DM_SliderFill);
	SliderStyle.HoveredBarImage.SetResourceObject(DM_SliderFill);
	
	ValueSlider->SetWidgetStyle(SliderStyle);
}

void UCodeSoundWidget::OnSliderChangedValue(const float Value)
{
	UpdateSlider(Value);
	OnVolumeSliderValueChanged.Broadcast(Value);
}

void UCodeSoundWidget::UpdateSlider(const float Value)
{
	DM_SliderFill->SetScalarParameterValue(TEXT("Percent"), Value);
	ValueText->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(Value * 100))));
	ValueSlider->SetValue(Value);
}

float UCodeSoundWidget::GetVolumeValue()
{
	return ValueSlider->GetValue();
}
