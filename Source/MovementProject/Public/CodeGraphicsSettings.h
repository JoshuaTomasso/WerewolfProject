// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "CodeProjectGraphicsConfig.h"
#include "CodeGraphicsSettings.generated.h"

class UCodeOptionCycle;

UCLASS()
class MOVEMENTPROJECT_API UCodeGraphicsSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ResetButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ApplyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SettingsScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	class UDataTable* GraphicsSettingsDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeOptionCycle> OptionCycleWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* OptionCycleWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* OverallOption;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TArray<UCodeOptionCycle*> OtherOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	TMap<ECodeProjectGraphicOptions, UCodeOptionCycle*> AllOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other")
	int32 DefaultOverallIndex;
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void ResetButtonClicked();
	
	UFUNCTION()
	void ApplyButtonClicked();
	
	UFUNCTION()
	void BackButtonClicked();
	
	UFUNCTION()
	void SetupGraphicsSettings();
	
	UFUNCTION()
	void ChangeOverallOptions(int32 SelectedIndex);
	
	UFUNCTION()
	void OnNonOverallOptionChanged(int32 SelectedIndex);
	
	UFUNCTION()
	void ApplyOptions();
	
	UFUNCTION()
	void ResetOptions();
	
	UFUNCTION(BlueprintPure, Category = "Other")
	int32 GetOptionValue(ECodeProjectGraphicOptions GraphicOptionType) const;
};
