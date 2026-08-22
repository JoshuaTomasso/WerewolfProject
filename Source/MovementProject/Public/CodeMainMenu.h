// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "CodeMainMenuPanelOrder.h"
#include "CodeMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API UCodeMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainMenuWidgetSwitcher;
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowPanel(ECodeMainMenuPanelOrder PanelOrder);
	
protected:
	virtual void NativeConstruct() override;
	

	
};
