// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CodeMainMenu.h"
#include "CodeLoadingScreen.h"

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

void ACodeMainMenuPlayerController::ShowLoadingScreen()
{
	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<UCodeLoadingScreen>(this, LoadingScreenWidgetClass);
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->AddToViewport(9999);
		}
		
	}
}
