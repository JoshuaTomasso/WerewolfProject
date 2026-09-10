// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "CodeOptionRotator.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeOptionRotator : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Text")
	FText OR_ChangeableText;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPips;
	
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SetupPips(int32 Count);
	
	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SetActivePipIndex(int32 Index); 
	
	UFUNCTION()
	void MarkAsCustom();

protected:
	
	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void HandleRotated(int32 Value, ERotatorDirection RotatorDir);
	
};
