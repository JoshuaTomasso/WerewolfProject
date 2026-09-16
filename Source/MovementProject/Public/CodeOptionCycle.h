// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodeOptionCycle.generated.h"

class UCodeMainWidgetButton;
class UCodeOptionRotator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionChanged, int32, SelectedIndex);

UCLASS()
class MOVEMENTPROJECT_API UCodeOptionCycle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_OptionName;
	
	UPROPERTY(meta = (BindWidget))
	UCodeMainWidgetButton* RightButton;
	
	UPROPERTY(meta = (BindWidget))
	UCodeOptionRotator* OptionText;
	
	UPROPERTY(meta = (BindWidget))
	UCodeMainWidgetButton* LeftButton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Text")
	TArray<FText> OptionArray;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Text")
	int DefaultSelectedIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Text")
	FText OptionNameText;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boolean")
	bool bIsMarkedAsCustom = false;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnOptionChanged OnOptionChanged;
	
protected:
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Buttons")
	void OnLeftButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Buttons")
	void OnRightButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Rotator")
	void UpdateSelected(int32 SelectedIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Rotator")
	void UpdateTextToCustom();
	
	UFUNCTION(BlueprintCallable, Category = "Other")
	int32 GetSelectedIndex() const;
	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitializeOptionCycle();
	
};
