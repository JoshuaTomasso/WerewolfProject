// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECodePauseMenuPanelOrder.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "CodePauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodePauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* PauseMenuWidgetSwitcher;
	
public:
	virtual void NativeConstruct() override;
	
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowPanel(ECodePauseMenuPanelOrder PanelOrder);
	
	UFUNCTION(BlueprintCallable, Category = "Close")
	void ClosePauseMenu();
};
