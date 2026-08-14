// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CodeMainMenu.h"

void ACodeMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		if (MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UCodeMainMenu>(this, MainMenuWidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}
