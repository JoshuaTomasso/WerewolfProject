// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "CodeCommonButtonWithText.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeCommonButtonWithText : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_TextBlock;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	FText DefaultText;

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void SetButtonText(const FText& NewText);
	
protected:
	virtual void NativeConstruct() override;
	
};
