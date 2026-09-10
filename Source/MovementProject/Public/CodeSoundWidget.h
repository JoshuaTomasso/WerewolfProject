// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AnalogSlider.h"
#include "CodeSoundWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVolumeSliderValueChangedDelegate, float, Value);

UCLASS()
class MOVEMENTPROJECT_API UCodeSoundWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_OptionName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ValueText;
	
	UPROPERTY(meta = (BindWidget))
	UAnalogSlider* ValueSlider;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	float DefaultSliderValue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FText SoundName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	UMaterialInstanceDynamic* DM_SliderFill;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UMaterialInterface* SliderFillParentMaterial;
	
	UPROPERTY(BlueprintAssignable, Category = "Defaults")
	FOnVolumeSliderValueChangedDelegate OnVolumeSliderValueChanged;
	
protected:
	
	virtual void NativePreConstruct() override;
	
public:
	
	UFUNCTION()
	void SetMainSliderImage();
	
	UFUNCTION()
	void OnSliderChangedValue(float Value);
	
	UFUNCTION()
	void UpdateSlider(float Value);
	
	UFUNCTION()
	float GetVolumeValue();
	
};
