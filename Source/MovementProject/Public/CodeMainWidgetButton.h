// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "CodeMainWidgetButton.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeMainWidgetButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MB_Text;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Text")
	FText MB_ChangeableText;
	
protected:
	
	virtual void NativePreConstruct() override;
	
	
public:
	
	UFUNCTION()
	void SetButtonText(const FText& Text);
};
