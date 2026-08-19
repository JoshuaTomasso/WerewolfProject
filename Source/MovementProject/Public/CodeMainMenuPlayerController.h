// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CodeMainMenuPlayerController.generated.h"

class UCodeMainMenu;
class UCodeLoadingScreen;

/**
 * 
 */
UCLASS()
class MOVEMENTPROJECT_API ACodeMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UCodeMainMenu* MainMenuWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeMainMenu> MainMenuWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	UCodeLoadingScreen*  LoadingScreenWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UCodeLoadingScreen> LoadingScreenWidgetClass;
	
protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();
	
};