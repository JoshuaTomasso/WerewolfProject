// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "InputCoreTypes.h"
#include "CodeKeyButton.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeKeyButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_KeyName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FKey DefaultKey;
	
	
protected:
	virtual void NativePreConstruct() override;
	
public:
	
	UFUNCTION()
	void UpdateKey(const FKey& Key);
	
};
