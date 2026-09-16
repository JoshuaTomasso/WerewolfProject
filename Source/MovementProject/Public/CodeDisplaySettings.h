// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GenericPlatform/GenericWindow.h"
#include "ExtendedUserSettingsSubsystem.h"
#include "CodeDisplaySettings.generated.h"

class UCodeOptionCycle;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeDisplaySettings : public UUserWidget
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
	UScrollBox* DisplayScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeOptionCycle> OptionCycleWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* DisplayScreenWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* WindowedModeWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* ScreenResolutionWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCodeOptionCycle* FPSLimitWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Settings")
	TArray<TEnumAsByte<EWindowMode::Type>> AllWindowedModes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display Settings")
	TArray<FDisplayInfo> AllDisplays;
	
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
	void SetupDisplayScreenWidget();
	
	UFUNCTION()
	void SetupWindowedModeWidget();
	
	UFUNCTION()
	void SetupScreenResolutionWidget();
	
	UFUNCTION()
	void SetupFPSLimitWidget();
	
	UFUNCTION()
	void SetActiveDisplay();
	
	UFUNCTION()
	void SetResolution();
	
	UFUNCTION()
	void ApplyDisplaySettings();
	
};
