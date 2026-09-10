// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CodeKeyButton.h"
#include "InputCoreTypes.h"
#include "CodeOptionKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeyChangedDispatch, FName, InputName, FKey, InputKey);

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeOptionKey : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_OptionName;
	
	UPROPERTY(meta = (BindWidget))
	UCodeKeyButton* MainKey;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DefaultInputName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FKey DefaultInputKey;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsListeningToKeyEvent = false;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnKeyChangedDispatch OnKeyChangedDispatch;
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
public:
	
	UFUNCTION()
	void OnKeyButtonClicked();
	
	UFUNCTION()
	void SetUnAssigned();
	
};
